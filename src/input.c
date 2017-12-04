#include <stdio.h>
#include <SDL2/SDL.h>

#include "input.h"

int listener() {
    int direction;
    direction = getchar() - '0';
    switch (direction) {
        case 8:
            direction = DIR_UP;
            break;
        case 6:
            direction = DIR_RIGHT;
            break;
        case 2:
            direction = DIR_DOWN;
            break;
        case 4:
            direction = DIR_LEFT;
            break;
        default:
            return -1;
    }
    printf("dir: %d\n", direction);
    SDL_Event event;
    while (SDL_PollEvent(&event));
    if (event.type == SDL_KEYDOWN) {
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
                return -1;
        }
    }
    return direction;
}

void initialize(Game *game) {
    FILE *map;
    map = fopen("map.txt", "r");

    if (map == NULL) {
        printf("Damn The map file does\'t exists!\n");
        return;
    }
    fscanf(map, "%d %d\n", &game->stage.n, &game->stage.m);
    for (int i = 0; i < game->stage.n; ++i) {
        for (int j = 0; j < game->stage.m; ++j) {
            fscanf(map, "%c", &game->stage.tiles[i][j]);
        }
        fscanf(map, "\n");
    }
    game->stage.cycles = 0;
    game->stage.roomNumber = 1;
    pacmanInit(&game->pacman, map);
    ghostInit(&game->blinky, map);
    ghostInit(&game->pinky, map);
    ghostInit(&game->clyde, map);
    ghostInit(&game->inky, map);
    fclose(map);
}

void pacmanInit(Pacman *pacman, FILE *map) {
    fscanf(map, "%d %d",
           &pacman->coordinates.start.x,
           &pacman->coordinates.start.y);
    pacman->hearts = 3;
    pacman->coordinates.current.x = pacman->coordinates.start.x;
    pacman->coordinates.current.y = pacman->coordinates.start.y;
    pacman->coordinates.speed = PACMAN_NORMAL_SPEED;
}

void ghostInit(Ghost *ghost, FILE *map) {
    fscanf(map, "%d %d",
           &ghost->coordinates.start.x,
           &ghost->coordinates.start.y);
    ghost->coordinates.current.x = ghost->coordinates.start.x;
    ghost->coordinates.current.y = ghost->coordinates.start.y;
    ghost->defensiveSecondsLeft = 0;
    ghost->coordinates.speed = GHOST_AGGRESIVE_SPEED;
}
