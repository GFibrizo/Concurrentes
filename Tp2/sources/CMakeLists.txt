cmake_minimum_required(VERSION 3.2)
project(sources)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Makefile)

add_executable(sources ${SOURCE_FILES} message.h record.h queue.h)