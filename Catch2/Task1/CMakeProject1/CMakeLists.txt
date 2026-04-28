cmake_minimum_required(VERSION 3.14)
project(ListTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.4.0
)

FetchContent_MakeAvailable(Catch2)

add_executable(list_tests test_list.cpp)

target_link_libraries(list_tests PRIVATE Catch2::Catch2WithMain)