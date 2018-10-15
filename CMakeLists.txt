cmake_minimum_required(VERSION 3.12)
project(fcwrpg)

set(CMAKE_CXX_STANDARD 14)

IF(${WIN32})
    INCLUDE_DIRECTORIES(allegro_win/include)
    LINK_DIRECTORIES(allegro_win/lib)
ENDIF(${WIN32})

add_executable(fcwrpg
        main.cpp
        Sprite.cpp Sprite.h
        Textbox.cpp Textbox.h
        Map.cpp Map.h
        Tile.cpp Tile.h
)

add_dependencies(fcwrpg copy_deps)
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
)

IF(${WIN32})
    find_file(pthread libwinpthread-1.dll)
    find_file(libgcc libgcc_s_dw2-1.dll)
    find_file(libstdc libstdc++-6.dll)
ENDIF(${WIN32})

add_custom_target(copy_deps)
IF(${WIN32})
    add_custom_command(
            TARGET copy_deps
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ./resources
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/allegro_win/bin ./
            COMMAND ${CMAKE_COMMAND} -E copy ${pthread} ${libgcc} ${libstdc} ./
    )
ELSE(${WIN32})
    add_custom_command(
            TARGET copy_deps
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ./resources
    )
ENDIF(${WIN32})

add_custom_target(zip_package)
add_dependencies(zip_package copy_deps)

IF(${WIN32})
    add_custom_command(
            TARGET zip_package
            COMMAND ${CMAKE_COMMAND} -E tar "cvf" "package.zip" --format=zip --
            fcwrpg.exe
            resources
            allegro-5.2.dll
            allegro_acodec-5.2.dll
            allegro_audio-5.2.dll
            allegro_color-5.2.dll
            allegro_dialog-5.2.dll
            allegro_font-5.2.dll
            allegro_image-5.2.dll
            allegro_main-5.2.dll
            allegro_memfile-5.2.dll
            allegro_physfs-5.2.dll
            allegro_primitives-5.2.dll
            allegro_ttf-5.2.dll
            allegro_video-5.2.dll
            libwinpthread-1.dll
            libgcc_s_dw2-1.dll
            libstdc++-6.dll
    )
ELSE(${WIN32})
    add_custom_command(
            TARGET zip_package
            COMMAND ${CMAKE_COMMAND} -E tar "cvf" "package.zip" --format=zip --
            fcwrpg
            resources
    )
ENDIF(${WIN32})