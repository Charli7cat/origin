cmake_minimum_required(VERSION 3.10)
project(TCPClient)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-D_WIN32_WINNT=0x0600)
    
    set(PLATFORM_LIBS ws2_32)
    
else()
    set(PLATFORM_LIBS)
endif()

add_executable(tcp_client src/main.cpp)

target_link_libraries(tcp_client ${PLATFORM_LIBS})

if(MSVC)
    target_compile_options(tcp_client PRIVATE /W4)
else()
    target_compile_options(tcp_client PRIVATE -Wall -Wextra)
endif()

set_target_properties(tcp_client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)