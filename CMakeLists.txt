cmake_minimum_required(VERSION 3.1)

project(PEGParser)

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories("include/")

add_subdirectory(src)

enable_testing()

add_subdirectory(tests)
