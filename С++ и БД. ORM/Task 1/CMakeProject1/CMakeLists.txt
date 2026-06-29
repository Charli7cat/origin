cmake_minimum_required(VERSION 3.10)
project(BookShopORM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Wt REQUIRED)

find_package(PostgreSQL REQUIRED)

include_directories(${Wt_INCLUDE_DIRS})
include_directories(${PostgreSQL_INCLUDE_DIRS})

add_executable(bookshop_orm 
    main.cpp
)

target_link_libraries(bookshop_orm 
    ${Wt_LIBRARIES}
    ${PostgreSQL_LIBRARIES}
    wt
    wtdbo
    wtdbopostgres
)