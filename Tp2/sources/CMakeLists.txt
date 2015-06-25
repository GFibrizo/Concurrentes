cmake_minimum_required(VERSION 3.2)
project(sources)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES message.h record.h queue.h server.cpp server.h)

set(SERVER_FILES server.cpp server.h)

set(CLIENT_FILES )

add_executable(server ${SOURCE_FILES} ${SERVER_FILES} main_server.cpp)

add_executable(client ${SOURCE_FILES} ${CLIENT_FILES} main_client.cpp client.cpp client.h)