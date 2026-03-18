cmake_minimum_required(VERSION 3.10)
project(RaceSimulator)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(../RaceLibrary/include)

add_executable(RaceSimulator main.cpp ConsoleUI.h)

target_link_libraries(RaceSimulator PRIVATE RaceLibrary)

set_target_properties(RaceSimulator PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

message(STATUS "RaceSimulator built with RaceLibrary dependency")