#include<iostream>
#include<pqxx/pqxx>
#include<Windows.h>
#pragma execution_character_set("utf-8")


//�����, ��������� ��������� ��(�������).
//�����, ����������� �������� ������ �������.
//�����, ����������� �������� ������� ��� ������������� �������.
//�����, ����������� �������� ������ � �������.
//�����, ����������� ������� ������� � ������������� �������.
//�����, ����������� ������� ������������� �������.
//�����, ����������� ����� ������� �� ��� ������ � �����, �������, email ��� ��������.


//�������� ��������� ���� ������ ��� �������� ���������� � ��������
class ClientManager {
public:
    //�����������
    ClientManager() : connection(
        //�� ������� �������
        "dbname=HomeWork "
        "host=localhost "
        "port=5432 "
        "user=postgres "
        "password=Pizda1992 "
        "client_encoding=UTF8 ") {

        if (connection.is_open()) {
            std::cout << "Opened database successfully: " << connection.dbname() << std::endl;
            createTables();
        }
        else {
            std::cerr << "Can't open database" << std::endl;
        }
    }
    // �����, ��������� ��������� �� (�������)
    void createTables() {
        //��� ����, ����� �������� ������ � ������� ��� �������� ��������� ������,����������:
        //������� ������ work
        pqxx::work txn(connection);
        //������� ����� exec(), ������ � �������� ��������� ������ INSERT ��� UPDATE.2
        txn.exec(R"(
            CREATE TABLE IF NOT EXISTS clients (
                id SERIAL PRIMARY KEY,
                first_name VARCHAR(50),
                last_name VARCHAR(50),
                email VARCHAR(100),
                phone_numbers TEXT[]
            )
        )");
        //��������� ����������, ������ ����� commit().
        txn.commit();

        std::cout << "������� �������! ���!" << std::endl;
    }

    // �����, ����������� �������� ������ �������
    void addClient(const std::string& firstName, const std::string& lastName, const std::string& email) {
        pqxx::work txn(connection);
        txn.exec_params(
            "INSERT INTO clients (first_name, last_name, email) VALUES ($1, $2, $3)",
            firstName, lastName, email
        );
        txn.commit();

        std::cout << "������ ��������." << std::endl;
    }
    // �����, ����������� �������� ������� ��� ������������� �������
    void addPhoneNumber(int clientId, const std::string& phoneNumber) {
        pqxx::work txn(connection);
        txn.exec_params(
            "UPDATE clients SET phone_numbers = array_append(phone_numbers, $1) WHERE id = $2",
            phoneNumber, clientId
        );
        txn.commit();

        std::cout << "����� �������� ��������." << std::endl;
    }


    // �����, ����������� �������� ������ � �������
    void updateClient(int clientId, const std::string& firstName, const std::string& lastName, const std::string& email) {
        pqxx::work txn(connection);
        txn.exec_params(
            "UPDATE clients SET first_name = $1, last_name = $2, email = $3 WHERE id = $4",
            firstName, lastName, email, clientId
        );
        txn.commit();

        std::cout << "���������� ���������� ��������." << std::endl;
    }

    // �����, ����������� ������� ������� � ������������� �������
    void removePhoneNumber(int clientId, const std::string& phoneNumber) {
        pqxx::work txn(connection);
        txn.exec_params(
            "UPDATE clients SET phone_numbers = array_remove(phone_numbers, $1) WHERE id = $2",
            phoneNumber, clientId
        );
        txn.commit();

        std::cout << "����� �������� �����." << std::endl;
    }

    // �����, ����������� ������� ������������� �������
    void removeClient(int clientId) {
        pqxx::work txn(connection);
        txn.exec_params("DELETE FROM clients WHERE id = $1", clientId);
        txn.commit();

        std::cout << "������ �����." << std::endl;
    }

    // �����, ����������� ����� ������� �� ��� ������ � �����, �������, email ��� ��������
    void findClient(const std::string& searchValue) {
        pqxx::work txn(connection);
        pqxx::result result = txn.exec_params(
            "SELECT * FROM clients WHERE first_name = $1 OR last_name = $1 OR email = $1 OR $1 = ANY(phone_numbers)",
            searchValue
        );

        for (const auto& row : result) {
            std::cout << "������ ID: " << row["id"].as<int>() << std::endl;
            std::cout << "���: " << row["first_name"].as<std::string>() << std::endl;
            std::cout << "�������: " << row["last_name"].as<std::string>() << std::endl;
            std::cout << "Email: " << row["email"].as<std::string>() << std::endl;

            auto phoneNumbersField = row["phone_numbers"];
            if (!phoneNumbersField.is_null()) {
                auto phoneNumbers = phoneNumbersField.as<std::string>();
                std::cout << "�������: " << phoneNumbers << std::endl;
            }
            else {
                std::cout << "������� �����������." << std::endl;
            }
        }

        if (result.empty()) {
            std::cout << "������ �����������." << std::endl;
        }
    }


private:
    pqxx::connection connection;
    };




int main() {

    //std::setlocale(LC_ALL, "RU");
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);

    ClientManager clientManager;

    // ������ ������������� �������
    clientManager.addClient("����", "�����", "Zenya@example.com");
    clientManager.addClient("����", "�����", "Zorin@example.com");
    clientManager.addClient("���", "�����", "Tor@example.com");
    clientManager.addClient("�����", "���������", "Alesya@example.com");
    clientManager.addPhoneNumber(2, "+55555");
    clientManager.updateClient(3, "bbbb", "bbbbb", "ludka@example.com");
    clientManager.removePhoneNumber(1, "+5555555");
    clientManager.removeClient(1);
    clientManager.findClient("Alesya@example.com");

    return 0;


	/*//std::cout << "hell";
	//��� ����������
	try {
		pqxx::connection con(
			//�� ������� �������
			"dbname=HomeWork "
			"host=localhost "
			"port=5432 "
			"user=postgres "
			"password=Student20 ");
        if (con.is_open()) {
            std::cout << "Opened database successfully: " << con.dbname() << std::endl;

            // ������� ������� clients
            pqxx::work txn(con);
            txn.exec(R"(
                CREATE TABLE IF NOT EXISTS clients (
                    id SERIAL PRIMARY KEY,
                    first_name VARCHAR(50),
                    last_name VARCHAR(50),
                    email VARCHAR(100),
                    phone_numbers TEXT[]
                )
            )");
            txn.commit();
        }
       
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}*/
}