cmake_minimum_required(VERSION 3.22.1) 
project(solver_lib)				
set(SOURCE_LIB solver.cpp solver.h)		
add_library(solver_lib STATIC ${SOURCE_LIB})
