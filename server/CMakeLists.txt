cmake_minimum_required(VERSION 3.10)

project(pgw_server)

set(SOURCES 
    server_main.cpp
    server.cpp
    PGW/src/control_plane.cpp
    PGW/src/data_plane.cpp
    PGW/src/IMSI_handler.cpp
    PGW/src/session.cpp
    FileHandler/FileHandler.cpp
    FileHandler/functions.cpp
    ../BCD/bcd.cpp
)

set(HEADERS
    server.h
    ../BCD/bcd.h
    PGW/src/control_plane.h
    PGW/src/data_plane.h
    PGW/src/IMSI_handler.h
    PGW/src/session.h
    FileHandler/FileHandler.h
    FileHandler/functions.h
    ../json.hpp
    ../httplib.h
)

add_executable(pgw_server ${SOURCES} ${HEADERS})

