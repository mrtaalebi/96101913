cmake_minimum_required(VERSION 3.9)
project(pacman C)

set(CMAKE_C_STANDARD 99)
set(SOURCE src/main.c src/input.c src/input.h src/view.c src/view.h src/models.h src/models.c src/controler.c src/controler.h)
add_executable(pacman "${SOURCE}")
include_directories(src)


find_library(SDL2_LIBRARY NAME SDL2)
find_library(SDL2IMAGE_LIBRARY NAME SDL2IMAGE)
#target_link_libraries(pacman "${SDL2_LIBRARY}")


INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED)


include_directories("{SDL2_INCLUDE_DIRECTORY}")
include_directories("{SDL2IMAGE_INCLUDE_DIRECTORY}")
target_link_libraries(pacman "${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES}")