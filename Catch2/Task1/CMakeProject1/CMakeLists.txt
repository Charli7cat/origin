cmake_minimum_required(VERSION 3.14)
project(LinkedListTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test_list 
    test_list.cpp
    catch_amalgamated.cpp
)

if(MSVC)
    target_compile_options(test_list PRIVATE /W4)
else()
    target_compile_options(test_list PRIVATE -Wall -Wextra)
endif()