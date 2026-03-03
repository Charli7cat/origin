add_library(LeaverLib SHARED)

target_sources(LeaverLib PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/leaver.cpp
)

target_include_directories(LeaverLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(LeaverLib PRIVATE
    LEAVER_LIB_EXPORTS
)

if(WIN32)
    set_target_properties(LeaverLib PROPERTIES
        PREFIX ""
    )
endif()