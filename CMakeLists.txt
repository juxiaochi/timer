cmake_minimum_required(VERSION 3.15)

project(timer)

add_compile_options(-std=c++17)
add_compile_options(-g -Wall -O1 -Os)

include_directories(include)

aux_source_directory(. SRC_LIST)
aux_source_directory(src SRC_LIST)

add_executable(timer ${SRC_LIST})
target_link_libraries(timer pthread)
