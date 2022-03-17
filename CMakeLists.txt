#https://cmake.org/cmake/help/latest/guide/tutorial/A%20Basic%20Starting%20Point.html

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# set the project name
project(lcd)

# set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_VERBOSE_MAKEFILE FALSE)
set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_FLAGS -fconcepts-ts)

# add the executable
add_executable(lcd main.c src/lcd.c )

#Include directories
target_include_directories(lcd PUBLIC "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/src" )

