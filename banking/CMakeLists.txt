cmake_minimum_required(VERSION 3.0)
project(banking)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(banking
    Transaction.cpp
    Account.cpp
)

target_include_directories(banking PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(banking PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

install(
    TARGETS banking
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
