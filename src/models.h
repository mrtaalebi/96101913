#ifndef MODELS_H
#define MODELS_H

#define TILE 18

#include <stdbool.h>

#include <SDL2/SDL.h>

extern const int SCREEN_WIDTH, SCREEN_HEIGHT;

extern const int CYCLES_PER_SECOND, GHOST_MAX_DEFENSIVE_TIME_SECONDS, SCORE_ON_CYCLES,
        NEW_STAGE_DELAY;

extern const SDL_Color COLOR_CHEESE, COLOR_CHERRY, COLOR_PINEAPPLE, COLOR_PACMAN, COLOR_BLINKY, COLOR_PINKY, COLOR_INKY,
        COLOR_CLYDE, COLOR_BLUE_GHOST, COLOR_EYE, COLOR_PEOPLE, COLOR_STATISTICS, COLOR_EAT_GOODS, COLOR_EAT_GHOST, COLOR_READY;

extern const double CHARACTER_TO_TILE;

const enum Icons {
    EMPTY = '_', BLOCK = '#', CHEESE = '.', CHERRY = '^', PINEAPPLE = 'O',
};

extern const enum gameConditions {
    CONTINUE = 1, NEW_STAGE = 2, GAME_OVER = 3,
};

extern const int PACMAN_CYCLES_PER_MOVE, GHOST_AGGRESSIVE_CYCLES_PER_MOVE, GHOST_DEFENSIVE_CYCLES_PER_MOVE;

extern const int CHEESE_VALUE, CHERRY_VALUE, GHOST_EAT_VALUE, PINEAPPLE_VALUE;

enum {
    KEY_ESCAPE = 10, KEY_RETURN = 20,
};

enum {
    DIR_UP = 1, DIR_RIGHT = 2, DIR_DOWN = 3, DIR_LEFT = 4, DIR_NONE = 5,
} typedef Direction;

enum {
    CHARACTER_PACMAN, CHARACTER_BLINKY, CHARACTER_PINKY, CHARACTER_CLYDE, CHARACTER_INKY
} typedef CharacterType;

enum {
    PLAYING_GAME = 100, PAUSE_MENU = 200, HALL_OF_FAME = 300, CREDITS = 400, HELP = 500, EXIT = 600,
} typedef NowOn;

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
    NowOn nowOn;
} typedef Game;

struct {
    char* string;
    SDL_Surface* surface;
    SDL_Color* textColor;
} typedef Label;

struct {
    SDL_Surface* surface;
    Label* label;
}typedef Button;

#endif
