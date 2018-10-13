cmake_minimum_required(VERSION 3.12)
project(fcwrpg)

set(CMAKE_CXX_STANDARD 14)

#INCLUDE_DIRECTORIES(/usr/include)
#LINK_DIRECTORIES(/usr/lib)

INCLUDE_DIRECTORIES(allegro_win/include)
LINK_DIRECTORIES(allegro_win/lib)

add_executable(fcwrpg main.cpp sprite.cpp)
add_custom_command(
        TARGET fcwrpg
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/res ./res
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/allegro_win/bin ./
)
TARGET_LINK_LIBRARIES(fcwrpg allegro allegro_font allegro_ttf allegro_image)

find_file(pthread libwinpthread-1.dll)
find_file(libgcc libgcc_s_dw2-1.dll)
find_file(libstdc libstdc++-6.dll)

add_custom_target(zip_package)
add_custom_command(
        TARGET zip_package
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/res ./res
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/allegro_win/bin ./
        COMMAND ${CMAKE_COMMAND} -E copy ${pthread} ${libgcc} ${libstdc} ./
        COMMAND ${CMAKE_COMMAND} -E tar "cvf" "package.zip" --format=zip --
        fcwrpg.exe
        res
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