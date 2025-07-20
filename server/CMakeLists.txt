cmake_minimum_required(VERSION 3.10)

project(pgw_server)

set(SOURCES 
    server_main.cpp
    server.cpp
    ../PGW/src/bearer.cpp
    ../PGW/src/control_plane.cpp
    ../PGW/src/data_plane.cpp
    ../PGW/src/pdn_connection.cpp
    ../PGW/src/rate_limiter.cpp
)

set(HEADERS
    server.h
    PGW/src/bearer.h
    PGW/src/control_plane.h
    PGW/src/data_plane.h
    PGW/src/pdn_connection.h
    PGW/src/rate_limiter.h
)

add_executable(pgw_server ${SOURCES} ${HEADERS})

