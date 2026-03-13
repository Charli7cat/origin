cmake_minimum_required(VERSION 3.10)
project(RaceLibrary)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

# Простейший тестовый файл
file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/src/testlib.cpp" 
"#include <iostream>
extern \"C\" __declspec(dllexport) void test() {
    std::cout << \"Test\" << std::endl;
}")

add_library(RaceLibrary SHARED src/testlib.cpp)