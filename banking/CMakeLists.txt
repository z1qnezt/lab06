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
)

add_executable(tests
    ../tests/tests.cpp 
)

target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Создаем отдельную директорию для сборки GTest
set(GTEST_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/gtest_build)
file(MAKE_DIRECTORY ${GTEST_BUILD_DIR})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../third-party/gtest ${CMAKE_CURRENT_BINARY_DIR}/gtest_build)

target_link_libraries(tests
    banking
    gtest
    gtest_main
    pthread
)

add_test(NAME test_all COMMAND tests)
