cmake_minimum_required(VERSION 3.10)

project(pgw_client)

set(SOURCES 
    client_main.cpp
    client.cpp
)

set(HEADERS 
    client.h
    ../json.hpp
)

add_executable(pgw_client ${SOURCES} ${HEADERS})