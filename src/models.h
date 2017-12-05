#ifndef MODELS_H
#define MODELS_H

extern const int CYCLES_PER_SECOND, DELAY_MADE_WHEN_PACMAN_KILLS_A_GHOST, GHOST_MAX_DEFENSIVE_TIME;

extern const enum Icons {
    EMPTY = '_', BLOCK = '#', CHEESE = '.', CHERRY = '^', PINEAPPLE = 'O',
};

extern const enum gameConditions {
    CONTINUE = 1, NEW_STAGE = 2, GAME_OVER = 3,
};

extern const double PACMAN_NORMAL_SPEED, GHOST_AGGRESSIVE_SPEED, GHOST_DEFENSIVE_SPEED;

extern const int CHEESE_VALUE, CHERRY_VALUE, GHOST_EAT_VALUE;

enum {
    DIR_UP = 1, DIR_RIGHT = 2, DIR_DOWN = 3, DIR_LEFT = 4,
} typedef Direction;

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
    Point start;
    Point current;
    Direction direction;
    double speed;
} typedef Coordinates;

struct {
    Coordinates coordinates;
    Score score;
    int hearts;
} typedef Pacman;

struct {
    Coordinates coordinates;
    int defensiveSecondsLeft;
} typedef Ghost;

struct {
    char tiles[100][100];
    int n, m;
    int cycles;
    int roomNumber;
} typedef Stage;

struct {
    Pacman pacman;
    Ghost blinky;
    Ghost pinky;
    Ghost clyde;
    Ghost inky;
    Stage stage;
} typedef Game;

#endif
