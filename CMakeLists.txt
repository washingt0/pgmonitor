cmake_minimum_required(VERSION 3.16)
project(pgmonitor)

set(CMAKE_CXX_STANDARD 14)

include_directories(./src)

add_executable(pgmonitor
        src/main.cpp
        src/procs.cpp
        src/procs.hpp
        src/pg.hpp
        src/pg.cpp)

target_link_libraries(pgmonitor pq pqxx jsoncpp)
