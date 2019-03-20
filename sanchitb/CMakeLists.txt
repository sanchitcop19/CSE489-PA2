cmake_minimum_required(VERSION 3.12)
project(sanchitb)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

include_directories(.)
include_directories(include)

add_executable(sanchitb
        include/simulator.h
        src/abt.cpp
        src/simulator.cpp
        abt
        )

add_executable(gbn
        include/simulator.h
        src/gbn.cpp
        src/simulator.cpp
        gbn
        )


add_executable(sr
        include/simulator.h
        src/sr.cpp
        src/simulator.cpp
        sr
        )