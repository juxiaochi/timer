cmake_minimum_required(VERSION 3.15)

project(timer CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

add_compile_options(-g -Wall)

# 开启内存检测
if (CHECK_MEMORY STREQUAL "Y")
    message(STATUS "====> CHECK_MEMORY: ${CHECK_MEMORY}")
    add_compile_options(-fsanitize=address)
    link_libraries(-fsanitize=address)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O2)
endif()

# 设置debug版本 开启TIMER_DEBUG宏
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DTIMER_DEBUG")

include_directories(include)

aux_source_directory(. SRC_LIST)
aux_source_directory(src SRC_LIST)

add_executable(timer ${SRC_LIST})
target_link_libraries(timer pthread)
