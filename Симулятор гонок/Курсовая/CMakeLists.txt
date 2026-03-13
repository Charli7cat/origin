cmake_minimum_required(VERSION 3.10)
project(RaceSimulator)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(RaceLibrary/include)

set(SOURCES
    RaceLibrary/src/Camel.cpp
    RaceLibrary/src/FastCamel.cpp
    RaceLibrary/src/Centaur.cpp
    RaceLibrary/src/AllTerrainBoots.cpp
    RaceLibrary/src/MagicCarpet.cpp
    RaceLibrary/src/Eagle.cpp
    RaceLibrary/src/Broom.cpp
    RaceSimulator/main.cpp
)

add_executable(RaceSimulator ${SOURCES})

message(STATUS "Building with sources: ${SOURCES}")