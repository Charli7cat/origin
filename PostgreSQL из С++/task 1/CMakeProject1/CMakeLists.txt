cmake_minimum_required(VERSION 3.10)
project(ClientManager)

set(CMAKE_CXX_STANDARD 17)

find_package(SQLite3 REQUIRED)

include_directories(${SQLite3_INCLUDE_DIRS})

add_executable(client_manager 
    main.cpp 
    ClientManager.cpp
)

target_link_libraries(client_manager ${SQLite3_LIBRARIES})