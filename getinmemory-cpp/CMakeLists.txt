cmake_minimum_required(VERSION 2.8.4)

set(TARGET_NAME getinmemory_cpp)

project(${TARGET_NAME})

set(SOURCE_FILES main.cpp)

include_directories("/usr/local/include")

link_directories("/usr/local/lib")

add_executable(${TARGET_NAME} ${SOURCE_FILES})

target_link_libraries(${TARGET_NAME} "libcurl.a")
