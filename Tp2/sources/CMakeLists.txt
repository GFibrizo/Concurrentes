cmake_minimum_required (VERSION 3.1)
project (ConcusQl)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(sources)
add_executable(message queue client server database.cpp database.h database_record.cpp database_record constants)