cmake_minimum_required(VERSION 3.10)

project(pgw_client)

set(SOURCES 
    client_main.cpp
    client.cpp
    ../libs/BCD/bcd.cpp
)

set(HEADERS 
    client.h
    ../libs/BCD/bcd.h
    ../libs/json.hpp
)

add_executable(pgw_client ${SOURCES} ${HEADERS})