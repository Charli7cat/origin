cmake_minimum_required(VERSION 3.10)
project(RaceLibrary)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(LIBRARY_SOURCES
    src/Camel.cpp
    src/FastCamel.cpp
    src/Centaur.cpp
    src/AllTerrainBoots.cpp
    src/MagicCarpet.cpp
    src/Eagle.cpp
    src/Broom.cpp
)

add_library(RaceLibrary SHARED ${LIBRARY_SOURCES})

target_compile_definitions(RaceLibrary PRIVATE RACELIBRARY_EXPORTS)

set_target_properties(RaceLibrary PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

message(STATUS "RaceLibrary built as SHARED library")