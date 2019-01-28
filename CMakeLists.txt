cmake_minimum_required(VERSION 3.12)
project(fcwrpg)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_VERBOSE_MAKEFILE  ON)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

IF(${WIN32})
    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/libs/allegro_win/include ${CMAKE_SOURCE_DIR}/include)
    LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/libs/allegro_win/lib)
ENDIF()

set(SOURCE_FILES
        Main.cpp Main.h
        Sprite.cpp Sprite.h
        Map.cpp Map.h
        BoundingBox.cpp BoundingBox.h
        Util.cpp Util.h
        Animation.cpp Animation.h
        Music.cpp Music.h
        Event.cpp Event.h
        Object.cpp Object.h
        Keyboard.cpp Keyboard.h
        Engine.cpp Engine.h
        Log.cpp Log.h
        Options.cpp Options.h Thread.cpp Thread.h)

add_executable(fcwrpg ${SOURCE_FILES})
add_library(duktape STATIC ${CMAKE_SOURCE_DIR}/libs/duktape/duktape.c)
target_link_libraries(duktape m)# dl rt)

IF(${WIN32})
    add_dependencies(fcwrpg copy_win_deps)
ELSE()
    add_dependencies(fcwrpg copy_linux_deps)
ENDIF()

IF(${WIN32})
    TARGET_LINK_LIBRARIES(fcwrpg
            allegro_monolith
            stdc++fs
            duktape
    )
ELSE()
    TARGET_LINK_LIBRARIES(fcwrpg
            allegro
            allegro_acodec
            allegro_audio
            allegro_color
            allegro_dialog
            allegro_font
            allegro_image
            allegro_main
            allegro_memfile
            allegro_physfs
            allegro_primitives
            allegro_ttf
            allegro_video
            stdc++fs
            duktape
            pthread
    )
ENDIF()

SET(libgcc ${CMAKE_SOURCE_DIR}/libs/libgcc_s_dw2-1.dll)
SET(libstdc++ ${CMAKE_SOURCE_DIR}/libs/libstdc++-6.dll)
SET(pthread ${CMAKE_SOURCE_DIR}/libs/libwinpthread-1.dll)

add_custom_target(copy_win_deps)
add_custom_command(
        TARGET copy_win_deps
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ./resources
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/libs/allegro_win/bin/allegro_monolith-5.2.dll ./
        COMMAND ${CMAKE_COMMAND} -E copy ${libgcc} ${libstdc++} ${pthread} ./
)

add_custom_target(copy_linux_deps)
add_custom_command(
        TARGET copy_linux_deps
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ./resources
)

add_custom_target(win_package)
add_dependencies(win_package copy_win_deps)
file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/resources)
add_custom_command(
        TARGET win_package
        COMMAND ${CMAKE_COMMAND} -E tar "cvf" "win_package.zip" --format=zip --
        fcwrpg.exe
        resources
        allegro_monolith-5.2.dll
        libgcc_s_dw2-1.dll
        libstdc++-6.dll
)

add_custom_target(linux_package)
add_dependencies(linux_package copy_linux_deps)
file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/resources)
add_custom_command(
        TARGET linux_package
        COMMAND ${CMAKE_COMMAND} -E tar "cvf" "linux_package.zip" --format=zip --
        fcwrpg
        resources
)
