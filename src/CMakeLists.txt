file(GLOB_RECURSE source_files "*.cpp")
list(REMOVE_ITEM source_files "main.cpp")
file(GLOB_RECURSE header_files "../include/*.hpp" "../include/*.tpp")

add_library(${PROJECT_NAME} ${header_files} ${source_files})

