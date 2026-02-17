cmake_minimum_required(VERSION 3.10)

project(CMakeProject1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(CMakeProject1
    main.cpp
    triangle.cpp
    right_triangle.cpp
    isosceles_triangle.cpp
    equilateral_triangle.cpp
    quadrilateral.cpp
    rectangle.cpp
    square.cpp
    parallelogram.cpp
    rhomb.cpp
)
