cmake_minimum_required (VERSION 3.1)
project (ConcusQl)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(sources)
add_executable(ConcusQl message queue server database database_record constants main_client main_server client logger lock_file event_handler)