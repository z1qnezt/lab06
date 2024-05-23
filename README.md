## Homework

### Задание
1. Создайте `CMakeList.txt` для библиотеки *banking*.
2. Создайте модульные тесты на классы `Transaction` и `Account`.
    * Используйте mock-объекты.
    * Покрытие кода должно составлять 100%.
3. Настройте сборочную процедуру на **TravisCI**.
4. Настройте [Coveralls.io](https://coveralls.io/).

# 1. Создайте `CMakeList.txt` для библиотеки *banking*.
```
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" OFF)

project(banking)

add_library(banking STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/banking/Transaction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/banking/Account.cpp
)

target_include_directories(banking PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/banking
)

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(third-party/gtest)
  file(GLOB BANKING_TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/tests.cpp)
  add_executable(check ${BANKING_TEST_SOURCES})
  target_link_libraries(check banking gtest_main)
  add_test(NAME check COMMAND check)
endif()
```
# 2. Создайте модульные тесты на классы `Transaction` и `Account`.
```
#include <Account.h>
#include <gtest/gtest.h>
#include <Transaction.h>

TEST(Account, Banking){
    Account test(0,0);
    
    ASSERT_EQ(test.GetBalance(), 0);
    
    ASSERT_THROW(test.ChangeBalance(100), std::runtime_error);
    
    test.Lock();
    
    ASSERT_NO_THROW(test.ChangeBalance(100));
    
    ASSERT_EQ(test.GetBalance(), 100);

    ASSERT_THROW(test.Lock(), std::runtime_error);

    test.Unlock();
    ASSERT_THROW(test.ChangeBalance(100), std::runtime_error);
}

TEST(Transaction, Banking){
    const int base_A = 5000, base_B = 5000, base_fee = 100;

    Account Alice(0,base_A), Bob(1,base_B);
    Transaction test_tran;

    ASSERT_EQ(test_tran.fee(), 1);
    test_tran.set_fee(base_fee);
    ASSERT_EQ(test_tran.fee(), base_fee);

    ASSERT_THROW(test_tran.Make(Alice, Alice, 1000), std::logic_error);
    ASSERT_THROW(test_tran.Make(Alice, Bob, -50), std::invalid_argument);
    ASSERT_THROW(test_tran.Make(Alice, Bob, 50), std::logic_error);
    if (test_tran.fee()*2-1 >= 100)
        ASSERT_EQ(test_tran.Make(Alice, Bob, test_tran.fee()*2-1), false);

    Alice.Lock();
    ASSERT_THROW(test_tran.Make(Alice, Bob, 1000), std::runtime_error);
    Alice.Unlock();

    ASSERT_EQ(test_tran.Make(Alice, Bob, 1000), true);
    ASSERT_EQ(Alice.GetBalance(), base_A-1000-base_fee);
    ASSERT_EQ(Bob.GetBalance(), base_B+1000);

    ASSERT_EQ(test_tran.Make(Alice, Bob, 3900), false);
    ASSERT_EQ(Bob.GetBalance(), base_B+1000);
    ASSERT_EQ(Alice.GetBalance(), base_A-1000-base_fee);
}
```
# 3. Настройте сборочную процедуру на **TravisCI**.
```
name: actions

on:
 push:
  branches: [master]
 pull_request:
  branches: [master]

jobs: 
 build_Linux:

  runs-on: ubuntu-latest

  steps:
  - uses: actions/checkout@v3

  - name: Adding gtest
    run: git clone https://github.com/google/googletest.git third-party/gtest -b release-1.11.0

  - name: Install lcov
    run: sudo apt-get install -y lcov

  - name: Config banking with tests
    run: cmake -H. -B ${{github.workspace}}/build -DBUILD_TESTS=ON

  - name: Build banking
    run: cmake --build ${{github.workspace}}/build

  - name: Run tests
    run: build/check
```
# 4. Настройте [Coveralls.io](https://coveralls.io/).
# Обновления в CmakeLists.txt
```
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)                   
  add_compile_options(--coverage) 
endif()

project(banking)

add_library(banking STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/banking/Transaction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/banking/Account.cpp
)

target_include_directories(banking PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/banking
)

target_link_libraries(banking gcov)

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(third-party/gtest)
  file(GLOB BANKING_TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/tests.cpp)
  add_executable(check ${BANKING_TEST_SOURCES})
  target_link_libraries(check banking gtest_main)
  add_test(NAME check COMMAND check)
endif()

include(CTest)

if(BUILD_TESTS AND CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  include(CheckCXXCompilerFlag)
  check_cxx_compiler_flag("--coverage" HAS_COVERAGE)

  if(HAS_COVERAGE)
    target_compile_options(check PRIVATE "--coverage")
    target_link_libraries(check "--coverage")
  endif()

  set(COVERAGE_COMMAND "gcov")
  set(COVERAGE_FILE_PATTERN "*.cpp")
  set(COVERAGE_EXCLUDE_PATTERN "*/gtest/*")

  find_program(COVERAGE_COMMAND_PATH ${COVERAGE_COMMAND})

  if(NOT COVERAGE_COMMAND_PATH)
    message(FATAL_ERROR "${COVERAGE_COMMAND} not found!")
  endif()

  add_custom_target(coverage
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/coverage
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage
    COMMAND ${COVERAGE_COMMAND_PATH} ${COVERAGE_FILE_PATTERN}
    COMMAND ${COVERAGE_COMMAND_PATH} -r -o ${CMAKE_BINARY_DIR}/coverage ${COVERAGE_EXCLUDE_PATTERN}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating code coverage report"
  )
endif()
```
# Обновления в actions.yml
```
name: actions

on:
 push:
  branches: [master]
 pull_request:
  branches: [master]

jobs: 
 build_Linux:

  runs-on: ubuntu-latest

  steps:
  - uses: actions/checkout@v3

  - name: Adding gtest
    run: git clone https://github.com/google/googletest.git third-party/gtest -b release-1.11.0

  - name: Install lcov
    run: sudo apt-get install -y lcov

  - name: Config banking with tests
    run: cmake -H. -B ${{github.workspace}}/build -DBUILD_TESTS=ON

  - name: Build banking
    run: cmake --build ${{github.workspace}}/build

  - name: Run tests
    run: build/check

  - name: Do lcov stuff
    run: lcov -c -d build/CMakeFiles/banking.dir/banking/ --include *.cpp --output-file ./coverage/lcov.info

  - name: Publish to coveralls.io
    uses: coverallsapp/github-action@v1.1.2
    with:
```
