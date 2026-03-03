add_executable(ConsoleApp)

target_sources(ConsoleApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

target_include_directories(ConsoleApp PRIVATE
    ${CMAKE_SOURCE_DIR}/LeaverLib/include
)

target_link_libraries(ConsoleApp PRIVATE
    LeaverLib
)

if(WIN32)
    add_custom_command(TARGET ConsoleApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:LeaverLib>
            $<TARGET_FILE_DIR:ConsoleApp>
        COMMENT "Копирование LeaverLib.dll в папку с приложением"
    )
endif()