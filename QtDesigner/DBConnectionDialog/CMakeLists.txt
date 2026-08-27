cmake_minimum_required(VERSION 3.16)

project(DBConnectionDialog VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
cmake_minimum_required(VERSION 3.16)

project(DBConnectionDialog VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

if(NOT Qt6_FOUND)
    find_package(Qt5 REQUIRED COMPONENTS Widgets)
    set(QT_LIBRARIES Qt5::Widgets)
else()
    set(QT_LIBRARIES Qt6::Widgets)
endif()

qt_add_executable(DBConnectionDialog
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    ConnectionDialog.h
    ConnectionDialog.ui
    ConnectionDialog.cpp
)

target_link_libraries(DBConnectionDialog PRIVATE ${QT_LIBRARIES})

if(WIN32)
    set_target_properties(DBConnectionDialog PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
endif()