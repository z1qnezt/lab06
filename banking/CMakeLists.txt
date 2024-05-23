cmake_minimum_required(VERSION 3.0)
project(banking)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Добавьте библиотеку banking
add_library(banking
    Account.cpp
    Transaction.cpp
    # Другие исходные файлы вашей библиотеки banking
)

# Укажите пути для включения заголовочных файлов
target_include_directories(banking PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}  # Путь к заголовочным файлам библиотеки banking
)

# Добавьте исполняемый файл для модульных тестов
add_executable(tests
    test.cpp  # Ваш файл с тестами
)

# Укажите пути для включения заголовочных файлов библиотеки banking в тесты
target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}  # Путь к заголовочным файлам библиотеки banking
)

# Добавьте путь к папке с библиотекой Google Test
add_subdirectory(third-party/gtest)

# Добавление тестов и их зависимостей
target_link_libraries(tests
    banking
    gtest
    gtest_main
    pthread
)

# Добавление команды для запуска тестов
add_test(NAME test_all COMMAND tests)
