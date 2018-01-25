#include <stdio.h>
#include <stdbool.h>

#include "input.h"
#include "gui.h"
#include "models.h"

int listener() {
    SDL_Event event;
    int key_event = DIR_NONE;
    while (SDL_PollEvent(&event));
    SDL_Keycode key = event.key.keysym.sym;
    if (event.type == SDL_KEYDOWN) switch (key) {
        default:
            break;
        case SDLK_UP:
            key_event = DIR_UP;
            break;
        case SDLK_RIGHT:
            key_event = DIR_RIGHT;
            break;
        case SDLK_DOWN:
            key_event = DIR_DOWN;
            break;
        case SDLK_LEFT:
            key_event = DIR_LEFT;
            break;
        case SDLK_ESCAPE:
            key_event = KEY_ESCAPE;
            break;
        case SDLK_RETURN:
            key_event = KEY_RETURN;
            break;
    } else if (event.type == SDL_TEXTINPUT) {
        return event.text.text[0];
    }
    return key_event;
}

void initialize(Game *game) {
    FILE *map_txt;
    //map_txt = fopen("map.txt", "r");
    map_txt = fopen("pacman_map_wow.txt", "r");
    if (map_txt == NULL) {
        printf("Damn The map file does\'t exists!\n");
        return;
    }
    fscanf(map_txt, "%d %d\n", &game->stage.n, &game->stage.m);
    game->stage.numberOfFoods = 0;
    game->stage.cycles = 0;
    for (int i = 0; i < game->stage.n; ++i) {
        for (int j = 0; j < game->stage.m; ++j) {
            fscanf(map_txt, "%c", &game->stage.tiles[i][j]);
            if (game->stage.tiles[i][j] == CHEESE || game->stage.tiles[i][j] == PINEAPPLE)
                game->stage.numberOfFoods++;
        }
        fscanf(map_txt, "\n");
    }
    game->stage.roomNumber = 1;
    pacmanInit(&game->pacman, map_txt);
    ghostInit(&game->blinky, map_txt, CHARACTER_BLINKY);
    ghostInit(&game->pinky, map_txt, CHARACTER_PINKY);
    ghostInit(&game->clyde, map_txt, CHARACTER_CLYDE);
    ghostInit(&game->inky, map_txt, CHARACTER_INKY);
    fclose(map_txt);
}

void pacmanInit(Pacman *pacman, FILE *map_txt) {
    fscanf(map_txt, "%d %d",
           &pacman->coordinates.startPosition.x,
           &pacman->coordinates.startPosition.y);
    pacman->hearts = 4;
    pacman->score.totalScore = pacman->score.cheeseCount = pacman->score.cherryCount = pacman->score.ghostsKilled = pacman->score.pineappleCount = 0;
    pacman->coordinates.currentPosition.x = pacman->coordinates.startPosition.x;
    pacman->coordinates.currentPosition.y = pacman->coordinates.startPosition.y;
    pacman->coordinates.direction = DIR_NONE;
    pacman->coordinates.cyclesPerMove = PACMAN_CYCLES_PER_MOVE;
    pacman->coordinates.isDefensive = false;
    pacman->coordinates.waitedCycles = 0;
    pacman->coordinates.characterType = CHARACTER_PACMAN;
    characterCoordinatesToPixels(&pacman->coordinates);
}

void ghostInit(Ghost *ghost, FILE *map_txt, int ghostType) {
    fscanf(map_txt, "%d %d",
           &ghost->coordinates.startPosition.x,
           &ghost->coordinates.startPosition.y);
    ghost->coordinates.currentPosition.x = ghost->coordinates.startPosition.x;
    ghost->coordinates.currentPosition.y = ghost->coordinates.startPosition.y;
    ghost->defensiveCyclesLeft = 0;
    ghost->coordinates.direction = DIR_NONE;
    ghost->coordinates.cyclesPerMove = GHOST_AGGRESSIVE_CYCLES_PER_MOVE;
    ghost->coordinates.isDefensive = false;
    ghost->coordinates.waitedCycles = 0;
    ghost->coordinates.characterType = ghostType;
    characterCoordinatesToPixels(&ghost->coordinates);
}

void characterCoordinatesToPixels(Coordinates *coordinates) {
    coordinates->currentPosition.x *= TILE;
    coordinates->currentPosition.y *= TILE;
    coordinates->startPosition.x *= TILE;
    coordinates->startPosition.y *= TILE;
    coordinates->startPosition.y -= TILE / 2;
    coordinates->currentPosition.y -= TILE / 2;
}

Fame* readFames() {
    FILE* fames_txt = fopen("fames", "r");
    Fame *fames = malloc(sizeof(Fame) * 20);
    for (int i = 0; i < 10; ++i) {
        fames[i].name = malloc(20);
        fscanf(fames_txt, "%s %d %d %d %d %d\n", fames[i].name, &fames[i].score.totalScore, &fames[i].score.cheeseCount,
               &fames[i].score.pineappleCount, &fames[i].score.cherryCount, &fames[i].score.ghostsKilled);
    }
    fclose(fames_txt);
    return fames;
}


void writeFames(Fame newFames) {
    FILE* fames_txt = fopen("fames", "r");
    Fame *fames = malloc(sizeof(Fame) * 20);
    Fame min;
    int index = -1;
    min.score.totalScore = 1000000;
    min.name = malloc(sizeof(char) * 20);
    for (int i = 0; i < 10; ++i) {
        fames[i].name = malloc(20);
        fscanf(fames_txt, "%s %d %d %d %d %d\n", fames[i].name, &fames[i].score.totalScore, &fames[i].score.cheeseCount,
               &fames[i].score.pineappleCount, &fames[i].score.cherryCount, &fames[i].score.ghostsKilled);
        if (fames[i].score.totalScore < min.score.totalScore) {
            min = fames[i];
            index = i;
        }
    }
    fclose(fames_txt);
    if (newFames.score.totalScore > min.score.totalScore && index != -1) fames[index] = newFames;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (fames[i].score.totalScore < fames[i + 1].score.totalScore) {
                min = fames[i];
                fames[i] = fames[i + 1];
                fames[i + 1] = min;
            }
        }
    }
    fames_txt = fopen("fames", "w");
    for (int i = 0; i < 10; ++i) {
        fprintf(fames_txt, "%s %d %d %d %d %d\n", fames[i].name, fames[i].score.totalScore, fames[i].score.cheeseCount,
                fames[i].score.pineappleCount, fames[i].score.cherryCount, fames[i].score.ghostsKilled);
    }
    fclose(fames_txt);
}