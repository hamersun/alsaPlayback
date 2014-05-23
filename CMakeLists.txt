project(alsaPlayback)

add_executable(alsaPlayback main.cpp)

target_link_libraries(alsaPlayback
        debug -lasound
        optimized -lasound
        )
