#include <stdio.h>
#include <SDL2/SDL.h>

#include "input.h"

int listener() {
    int direction = -1;
    SDL_Event event;
    while (SDL_PollEvent(&event));
        SDL_Keycode key = event.key.keysym.sym;
        switch (key) {
            case SDLK_UP:
                direction = DIR_UP;
                break;
            case SDLK_RIGHT:
                direction = DIR_RIGHT;
                break;
            case SDLK_DOWN:
                direction = DIR_DOWN;
                break;
            case SDLK_LEFT:
                direction = DIR_LEFT;
                break;
            default:
                break;
        }
    return direction;
}

void initialize(Game *game) {
    FILE *map_txt;
    map_txt = fopen("map.txt", "r");

    if (map_txt == NULL) {
        printf("Damn The map file does\'t exists!\n");
        return;
    }
    fscanf(map_txt, "%d %d\n", &game->stage.n, &game->stage.m);
    for (int i = 0; i < game->stage.n; ++i) {
        for (int j = 0; j < game->stage.m; ++j) {
            fscanf(map_txt, "%c", &game->stage.tiles[i][j]);
        }
        fscanf(map_txt, "\n");
    }
    game->stage.roomNumber = 1;
    pacmanInit(&game->pacman, map_txt);
    ghostInit(&game->blinky, map_txt);
    ghostInit(&game->pinky, map_txt);
    ghostInit(&game->clyde, map_txt);
    ghostInit(&game->inky, map_txt);
    fclose(map_txt);
}

void pacmanInit(Pacman *pacman, FILE *map_txt) {
    fscanf(map_txt, "%d %d",
           &pacman->coordinates.start.x,
           &pacman->coordinates.start.y);
    pacman->hearts = 3;
    pacman->coordinates.current.x = pacman->coordinates.start.x;
    pacman->coordinates.current.y = pacman->coordinates.start.y;
    pacman->coordinates.speed = PACMAN_NORMAL_SPEED;
}

void ghostInit(Ghost *ghost, FILE *map_txt) {
    fscanf(map_txt, "%d %d",
           &ghost->coordinates.start.x,
           &ghost->coordinates.start.y);
    ghost->coordinates.current.x = ghost->coordinates.start.x;
    ghost->coordinates.current.y = ghost->coordinates.start.y;
    ghost->defensiveSecondsLeft = 0;
    ghost->coordinates.speed = GHOST_AGGRESSIVE_SPEED;
}
