cmake_minimum_required(VERSION 3.10)
project(pgw)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(pgw_lib
    src/control_plane.cpp
    src/data_plane.cpp
    src/IMSI_handler.cpp
)

target_link_libraries(pgw_lib
    PRIVATE Boost::boost
)

add_executable(pgw_main
    src/main.cpp
)

target_link_libraries(pgw_main
    PRIVATE pgw_lib
)