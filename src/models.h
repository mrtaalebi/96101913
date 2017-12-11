#ifndef MODELS_H
#define MODELS_H

#include <stdbool.h>

#include <SDL2/SDL.h>

extern const int SCREEN_WIDTH, SCREEN_HEIGHT, TILE;

extern const int CYCLES_PER_SECOND, DELAY_MADE_WHEN_PACMAN_KILLS_A_GHOST, GHOST_MAX_DEFENSIVE_TIME_SECONDS;

const enum Icons {
    EMPTY = '_', BLOCK = '#', CHEESE = '.', CHERRY = '^', PINEAPPLE = 'O',
};

extern const enum gameConditions {
    CONTINUE = 1, NEW_STAGE = 2, GAME_OVER = 3,
};

extern const int PACMAN_CYCLES_PER_MOVE, GHOST_AGGRESSIVE_CYCLES_PER_MOVE, GHOST_DEFENSIVE_CYCLES_PER_MOVE;

extern const int CHEESE_VALUE, CHERRY_VALUE, GHOST_EAT_VALUE;

enum {
    DIR_UP = 1, DIR_RIGHT = 2, DIR_DOWN = 3, DIR_LEFT = 4, DIR_NONE = -1,
} typedef Direction;

enum {
    CHARACTER_PACMAN, CHARACTER_BLINKY, CHARACTER_PINKY, CHARACTER_CLYDE, CHARACTER_INKY
} typedef CharacterType;

struct {
    int cheeseCount;
    int cherryCount;
    int pineappleCount;
    int ghostsKilled;
    int totalScore;
} typedef Score;

struct {
    int x;
    int y;
} typedef Point;

struct {
    int cyclesPerMove;
    int waitedCycles;
    CharacterType characterType;
    Point startPosition;
    Point currentPosition;
    Direction direction;
    Direction nextDirection;
    SDL_Texture* texture;
    bool isDefensive;
} typedef Coordinates;

struct {
    int hearts;
    Score score;
    Coordinates coordinates;
} typedef Pacman;

struct {
    int defensiveCyclesLeft;
    Coordinates coordinates;
} typedef Ghost;

struct {
    int n, m;
    int cycles;
    int roomNumber;
    char tiles[50][50];
    int numberOfFoods;
} typedef Stage;

struct {
    Ghost blinky;
    Ghost pinky;
    Ghost clyde;
    Ghost inky;
    Pacman pacman;
    Stage stage;
} typedef Game;

#endif
