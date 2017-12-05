cmake_minimum_required(VERSION 3.9)
project(pacman C)

set(CMAKE_C_STANDARD 99)
set(SOURCE src/main.c src/input.c src/input.h src/view.c src/view.h src/models.h src/models.c src/controler.c src/controler.h)
add_executable(pacman "${SOURCE}")
include_directories(src)

include_directories("/usr/include/SDL2")
target_link_libraries(pacman m SDL2 SDL2_gfx SDL2_image)

ADD_DEFINITIONS(-D_REENTRANT)
