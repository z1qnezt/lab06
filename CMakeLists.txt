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
