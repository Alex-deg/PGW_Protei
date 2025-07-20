cmake_minimum_required(VERSION 3.10)

project(pgw_server)

set(SOURCES 
    server_main.cpp
    server.cpp
)

set(HEADERS
    server.h
)

add_executable(pgw_server ${SOURCES} ${HEADERS})

