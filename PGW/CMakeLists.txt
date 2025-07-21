cmake_minimum_required(VERSION 3.10)
project(pgw)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(pgw_lib
    src/bearer.cpp
    src/control_plane.cpp
    src/data_plane.cpp
    src/pdn_connection.cpp
    src/rate_limiter.cpp
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

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test)
    enable_testing()
    
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
    
    add_executable(pgw_tests
        test/data_plane_test.cpp
        src/bearer.cpp
        src/control_plane.cpp
        src/data_plane.cpp
        src/pdn_connection.cpp
        src/rate_limiter.cpp
        src/IMSI_handler.cpp
    )
    
    target_link_libraries(pgw_tests
        PRIVATE pgw_lib
        ${GTEST_BOTH_LIBRARIES}
        pthread
    )
    
    add_test(NAME pgw_tests COMMAND pgw_tests)
endif()