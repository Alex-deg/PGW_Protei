cmake_minimum_required(VERSION 3.10)

project(pgw_server)

find_package(spdlog QUIET)

if(NOT spdlog_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
        GIT_TAG        "v1.11.0" 
    )
    FetchContent_MakeAvailable(spdlog)
endif()

set(SOURCES 
    server_main.cpp
    server.cpp
    PGW/src/control_plane.cpp
    PGW/src/data_plane.cpp
    PGW/src/session.cpp
    ../libs/FileHandler/FileHandler.cpp
    ../libs/FileHandler/functions.cpp
    ../libs/BCD/bcd.cpp
    ../libs/Logger/logger.cpp
    ../libs/ConfigParser/config_parser.cpp
)

set(HEADERS
    server.h
    ../libs/BCD/bcd.h
    PGW/src/control_plane.h
    PGW/src/data_plane.h
    PGW/src/session.h
    ../libs/FileHandler/FileHandler.h
    ../libs/FileHandler/functions.h
    ../libs/json.hpp
    ../libs/httplib.h
    ../libs/Logger/logger.h
    ../libs/ConfigParser/config_parser.h
)

add_executable(pgw_server ${SOURCES} ${HEADERS})
target_link_libraries(pgw_server PRIVATE spdlog::spdlog)
