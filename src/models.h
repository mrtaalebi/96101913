#ifndef MODELS_H
#define MODELS_H

#include <SDL_render.h>

const int SCREEN_WIDTH, SCREEN_HEIGHT, TILE;

const int CYCLES_PER_SECOND, DELAY_MADE_WHEN_PACMAN_KILLS_A_GHOST, GHOST_MAX_DEFENSIVE_TIME_SECONDS;

const enum Icons {
    EMPTY = '_', BLOCK = '#', CHEESE = '.', CHERRY = '^', PINEAPPLE = 'O',
};

const enum gameConditions {
    CONTINUE = 1, NEW_STAGE = 2, GAME_OVER = 3,
};

const int PACMAN_NORMAL_SPEED, GHOST_AGGRESSIVE_SPEED, GHOST_DEFENSIVE_SPEED;

const int CHEESE_VALUE, CHERRY_VALUE, GHOST_EAT_VALUE;

enum {
    DIR_UP = 1, DIR_RIGHT = 2, DIR_DOWN = 3, DIR_LEFT = 4, DIR_NONE = -1,
} typedef Direction;

enum {
    CHARACTER_PACMAN, CHARACTER_BLINKY, CHARACTER_PINKY, CHARACTER_CLYDE, CHARACTER_INKY,
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
    int speed;
    CharacterType characterType;
    Point startPosition;
    Point currentPosition;
    Direction direction;
    Direction nextDirection;
    SDL_Texture* texture;
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
    char background[2500][2500];
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
