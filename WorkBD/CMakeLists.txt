cmake_minimum_required(VERSION 3.5)

project(08_PreDataBase VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        database.h
        database.cpp
        dbdata.h
        dbdata.cpp
        dbdata.ui
)

add_executable(08_PreDataBase
    ${PROJECT_SOURCES}
)

set(PG_LIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/PG_Libs")

if(EXISTS "${PG_LIBS_DIR}/include")
    target_include_directories(${PROJECT_NAME} PRIVATE "${PG_LIBS_DIR}/include")
endif()

target_link_directories(${PROJECT_NAME} PRIVATE "${PG_LIBS_DIR}")

target_link_libraries(08_PreDataBase PRIVATE
        Qt6::Widgets
        Qt6::Sql
        libcrypto-3-x64
        libiconv-2
        libintl-9
        libpq
        libssl-3-x64
)

if(WIN32)
    file(GLOB PG_DLLS "${PG_LIBS_DIR}/*.dll")
    if(PG_DLLS)
        add_custom_command(TARGET 08_PreDataBase POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${PG_DLLS}
                    $<TARGET_FILE_DIR:08_PreDataBase>
        )
    endif()
endif()