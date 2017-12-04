

const int CYCLES_PER_SECOND = 60;
const int DELAY_MADE_WHEN_PACMAN_KILLS_A_GHOST = 5;
const int GHOST_MAX_DEFENSIVE_TIME = 10;

const enum Icons {
    EMPTY = '_',
    BLOCK = '#',
    CHEESE = '.',
    CHERRY = '^',
    PINEAPPLE = 'O',
};

const enum Conditions {
    CONTINUE = 1,
    NEW_STAGE = 2,
    GAME_OVER = 3,
};

const double PACMAN_NORMAL_SPEED = 1,
        GHOST_AGGRESIVE_SPEED = 1,
        GHOST_DEFENSIVE_SPEED = 0.5;

const int CHEESE_VALUE = 10,
        CHERRY_VALUE = 100,
        GHOST_EAT_VALUE = 20;

struct Score {
    int cheeseCount;
    int cherryCount;
    int pineappleCount;
    int ghostsKilled;
    int totalScore;
} typedef Score;

enum Direction {
    DIR_UP = 1, DIR_RIGHT = 2, DIR_DOWN = 3, DIR_LEFT = 4,
} typedef Direction;

struct Point {
    int x;
    int y;
} typedef Point;

struct Coordinates {
    Point start;
    Point current;
    Direction direction;
    double speed;
} typedef Coordinates;

struct Pacman {
    Coordinates coordinates;
    Score score;
    int hearts;
} typedef Pacman;

struct Ghost {
    Coordinates coordinates;
    int defensiveSecondsLeft;
} typedef Ghost;

struct Stage {
    char tiles[100][100];
    int n, m;
    int cycles;
    int roomNumber;
} typedef Stage;

struct Game {
    Pacman pacman;
    Ghost blinky;
    Ghost pinky;
    Ghost clyde;
    Ghost inky;
    Stage stage;
} typedef Game;
