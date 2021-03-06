cmake_minimum_required(VERSION 3.0)
project(fcwrpg)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_VERBOSE_MAKEFILE ON)

INCLUDE_DIRECTORIES(SYSTEM ${CMAKE_SOURCE_DIR}/lib/include)

IF(${WIN32})
    INCLUDE_DIRECTORIES(
            ${CMAKE_SOURCE_DIR}/lib/include
            ${CMAKE_SOURCE_DIR}/lib/windows/allegro/include
            ${CMAKE_SOURCE_DIR}/lib/windows/allegro_deps/include
    )
    LINK_DIRECTORIES(
            ${CMAKE_SOURCE_DIR}/lib/windows/libarchive
            ${CMAKE_SOURCE_DIR}/lib/windows/allegro/lib
            ${CMAKE_SOURCE_DIR}/lib/windows/allegro_deps/lib
    )
ELSE()

ENDIF()

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(fcwrpg ${SOURCES})
set_target_properties(fcwrpg PROPERTIES COMPILE_FLAGS "-Wall -Wextra -pedantic -ggdb3")
target_include_directories(fcwrpg PUBLIC "include/")
add_dependencies(fcwrpg copy_resources)

IF(${WIN32})
    add_dependencies(fcwrpg copy_win_deps)
ENDIF()

IF(${WIN32})
    TARGET_LINK_LIBRARIES(fcwrpg
            allegro_monolith-debug-static

            jpeg
            dumb
            FLAC
            freetype
            vorbisfile
            vorbis
            ogg
            physfs
            png16
            zlib
            opusfile
            opus

            dsound
            gdiplus
            uuid
            kernel32
            winmm
            psapi
            opengl32
            glu32
            user32
            comdlg32
            gdi32
            shell32
            ole32
            advapi32
            ws2_32
            shlwapi

            stdc++fs
            archive_static
    )
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libstdc++ -static-libgcc")
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
            #allegro_primitives
            allegro_ttf

            stdc++fs
            pthread
            archive
    )
    #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libstdc++ -static-libgcc")
ENDIF()

#SET(libgcc ${CMAKE_SOURCE_DIR}/lib/libgcc_s_dw2-1.dll)
#SET(libstdc++ ${CMAKE_SOURCE_DIR}/lib/libstdc++-6.dll)
#SET(pthread ${CMAKE_SOURCE_DIR}/lib/libwinpthread-1.dll)

add_custom_target(copy_win_deps)
add_custom_command(
        TARGET copy_win_deps
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/lib/windows/allegro/bin/allegro_monolith-5.2.dll ./
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/lib/windows/libarchive/libarchive.dll ./
)

add_custom_target(copy_resources)
add_custom_command(
        TARGET copy_resources
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/resources
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources
)

add_custom_target(win_package)
add_dependencies(win_package copy_win_deps)
add_dependencies(win_package fcwrpg)
add_custom_command(
        TARGET win_package
        COMMAND ${CMAKE_COMMAND} -E tar "cvf" "win_package.zip" --format=zip --
        fcwrpg.exe
        resources
        #allegro_monolith-5.2.dll
        #libgcc_s_dw2-1.dll
        #libstdc++-6.dll
        #libwinpthread-1.dll
)

add_custom_target(linux_package)
add_dependencies(linux_package fcwrpg)
add_custom_command(
        TARGET linux_package
        COMMAND ${CMAKE_COMMAND} -E tar "cvf" "linux_package.zip" --format=zip --
        fcwrpg
        resources
)
