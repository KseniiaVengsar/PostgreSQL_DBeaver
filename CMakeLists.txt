cmake_minimum_required(VERSION 3.20)
set(PostgreSQL_ADDITIONAL_VERSIONS "16")
project(HomeWork)
# путь к libpqxx-7.8.1
set(CMAKE_PREFIX_PATH "C:/Users/ksana/source/repos/libpqxx-7.8.1")
# добавить subdirectory
add_subdirectory("C:/Users/ksana/source/repos/libpqxx-7.8.1/libpqxx-7.8.1" libpqxx-build)
# перечисл файлы проекта
add_executable(HomeWork main.cpp)
# стандарт с++
target_compile_features(HomeWork PRIVATE cxx_std_17)
# линкуем с libpqxx
target_link_libraries(HomeWork pqxx)