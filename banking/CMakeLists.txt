cmake_minimum_required(VERSION 3.10)
project(banking)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(banking
    Account.cpp
    Transaction.cpp
)

target_include_directories(banking PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} 

# Добавьте исполняемый файл для модульных тестов
add_executable(tests
    test.cpp
)

target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR} 

add_subdirectory(third-party/gtest)

target_link_libraries(tests
    banking
    gtest
    gtest_main
    pthread
)

add_test(NAME test_all COMMAND tests)
