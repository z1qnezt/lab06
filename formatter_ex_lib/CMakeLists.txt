cmake_minimum_required(VERSION 3.22.1)

project(formatter_ex_lib)

set(SOURCE_LIB formatter_ex.cpp)

add_library(formatter_ex_lib STATIC ${SOURCE_LIB})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../formatter_lib formatter_lib)

target_link_libraries(formatter_ex_lib PUBLIC formatter_lib)
target_include_directories(formatter_ex_lib PUBLIC
				"${CMAKE_CURRENT_SOURCE_DIR}"
				"${CMAKE_CURRENT_SOURCE_DIR}/../formatter_lib")

