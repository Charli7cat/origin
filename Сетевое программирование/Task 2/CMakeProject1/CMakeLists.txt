cmake_minimum_required(VERSION 3.10)
project(TCPServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

if(WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-D_WIN32_WINNT=0x0600)
    set(PLATFORM_LIBS ws2_32)
else()
    set(PLATFORM_LIBS)
endif()

set(SOURCES
    src/tcp_server.cpp
    src/main.cpp
)

add_executable(tcp_server ${SOURCES})

target_link_libraries(tcp_server ${PLATFORM_LIBS})

if(MSVC)
    target_compile_options(tcp_server PRIVATE /W4)
else()
    target_compile_options(tcp_server PRIVATE -Wall -Wextra -pthread)
endif()

set_target_properties(tcp_server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)