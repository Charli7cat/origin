cmake_minimum_required(VERSION 3.10)
project(RaceSimulator)

set(CMAKE_CXX_STANDARD 11)

include_directories(../RaceLibrary/include)

add_executable(RaceSimulator main.cpp ConsoleUI.h)

target_link_libraries(RaceSimulator RaceLibrary)