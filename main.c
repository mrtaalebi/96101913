#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "input.h"

///////////////////////
/// constants start ///
///////////////////////

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ImplicitIntegerAndEnumConversion"

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

const double PACMAN_NORMAL_SPEED = 1,
        GHOST_AGGRESIVE_SPEED = 1,
        GHOST_DEFENSIVE_SPEED = 0.5;

const int CHEESE_VALUE = 10,
        CHERRY_VALUE = 100,
        GHOST_EAT_VALUE = 20;

/////////////////////
/// constants end ///
/////////////////////

////////////////////
/// models start ///
////////////////////

struct Score {
    int cheeseCount;
    int cherryCount;
    int pineappleCount;
    int ghostsKilled;
    int totalScore;
} typedef Score;

enum Direction {
    UP = 1, RIGHT = 2, DOWN = 3, LEFT = 4,
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
    Stage room;
} typedef Game;

//////////////////
/// models end ///
//////////////////

/////////////////////
/// headers start ///
/////////////////////

void initialize(Game *);

void pacmanInit(Pacman *, FILE *);

void gameReport(Game *);

int runACycle(Game *);

void makeANewRoom(Game *);

void moveACreature(Coordinates *, Stage *room);

void pacmanBecomeAHero(Game *);

int pacmanHitAGhost(Game *, Ghost *);

void checkPacmanAndGhostsCollision(Game *);

bool areOnTheSamePosition(Point, Point);

int checkRemainingFoods(Stage *);

void ghostBecomeDefensive(Ghost *);

void restartRoomByPacmanDeath(Game *);

void restartAGhostByPacmanDeath(Ghost *);

int getNextInCircular(int, int);

void ghostInit(Ghost *, FILE *);

void pacmanEat(Game *);

void log(char *);

///////////////////
/// headers end ///
///////////////////

//////////////////
/// main start ///
//////////////////

int runAGhostACycle(Game *game, Ghost *ghost) ;

int main() {
    Game game;
    initialize(&game);
    int condition;
    do {
        gameReport(&game);
        condition = runACycle(&game);
    } while (!condition);

    if (condition == 2) {
        gameReport(&game);
        printf("\ngreat! you made it!\n");
    } else if (condition == 1) {
        printf("\noops! you lose!\n");
    }
    printf("your final score is: %d\n", game.pacman.score.totalScore);
    return 0;
}

/// returns 0: continue current game
/// returns 1: game over launch standings
/// returns 2: win launch standings
int runACycle(Game *game) {
    int arrowKeyPressed = listener();
    game->pacman.coordinates.direction = arrowKeyPressed;
    game->room.cycles++;
    moveACreature(&game->pacman.coordinates, &game->room);
    pacmanEat(game);
    checkPacmanAndGhostsCollision(game);
    runAGhostACycle(game, &game->blinky);
    runAGhostACycle(game, &game->pinky);
    runAGhostACycle(game, &game->clyde);
    runAGhostACycle(game, &game->inky);
    int foodsLeft = checkRemainingFoods(&game->room);
    if (foodsLeft == 0 && game->pacman.hearts > 0) {
        return 2;
    } else if (game->pacman.hearts == 0) {
        return 1;
    } else return 0;
}

int runAGhostACycle(Game *game, Ghost *ghost) {
    if (ghost->defensiveSecondsLeft > 0) {
        ghost->defensiveSecondsLeft--;
    }
    Coordinates newOne = ghost->coordinates;
    newOne.direction = rand() % 4 + 1;
    moveACreature(&newOne, &game->room);
    for (int i = 0; i < 4; ++i) {
        if (areOnTheSamePosition(newOne.current, ghost->coordinates.current)) {
            newOne.direction = (newOne.direction + 1) % 4 + 1;
            moveACreature(&newOne, &game->room);
        } else break;
    }
    ghost->coordinates = newOne;
}


////////////////
/// main end ///
////////////////

/////////////////////
/// methods start ///
/////////////////////

void log(char * s) {
    printf("%s", s);
}

void initialize(Game *game) {
    FILE *map;
    map = fopen("map.txt", "r");

    if (map == NULL) {
        printf("Damn The map file does\'t exists!\n");
        return;
    }
    fscanf(map, "%d %d\n", &game->room.n, &game->room.m);
    for (int i = 0; i < game->room.n; ++i) {
        for (int j = 0; j < game->room.m; ++j) {
            fscanf(map, "%c", &game->room.tiles[i][j]);
        }
        fscanf(map, "\n");
    }
    game->room.cycles = 0;
    game->room.roomNumber = 1;
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

int checkRemainingFoods(Stage *room) {
    int foodsLeft = 0;
    for (int i = 0; i < room->n; ++i) {
        for (int j = 0; j < room->m; ++j) {
            char c = room->tiles[i][j];
            if (c == CHEESE || c == PINEAPPLE)
                foodsLeft++;
        }
    }
    return foodsLeft;
}

/// returns 0: stay
/// returns 1: pacman moved
void moveACreature(Coordinates *p, Stage *room) {
    Point newPoint = p->current;
    switch (p->direction) {
        case UP:
            newPoint.x = getNextInCircular(newPoint.x - 1, room->n);
            break;
        case RIGHT:
            newPoint.y = getNextInCircular(newPoint.y + 1, room->m);
            break;
        case DOWN:
            newPoint.x = getNextInCircular(newPoint.x + 1, room->n);
            break;
        case LEFT:
            newPoint.y = getNextInCircular(newPoint.y - 1, room->m);
            break;
    }
    char nextStep = room->tiles[newPoint.x][newPoint.y];
    if (nextStep != BLOCK) {
        p->current = newPoint;
    }
}

void pacmanEat(Game *game) {
    char pacmanOn = game->room.tiles[game->pacman.coordinates.current.x][game->pacman.coordinates.current.y];
    switch (pacmanOn) {
        case CHEESE:
            game->room.tiles[game->pacman.coordinates.current.x][game->pacman.coordinates.current.y] = EMPTY;
            game->pacman.score.cheeseCount++;
            game->pacman.score.totalScore += CHEESE_VALUE;
            break;
        case CHERRY:
            game->room.tiles[game->pacman.coordinates.current.x][game->pacman.coordinates.current.y] = EMPTY;
            game->pacman.score.cherryCount++;
            game->pacman.score.totalScore += CHERRY_VALUE;
            break;
        case PINEAPPLE:
            game->room.tiles[game->pacman.coordinates.current.x][game->pacman.coordinates.current.y] = EMPTY;
            game->pacman.score.pineappleCount++;
            pacmanBecomeAHero(game);
            break;
        default:
            break;
    }
}

int getNextInCircular(int index, int size) {
    if (index == -1) return  size - 1;
    else if (index >= 0 && index < size) return index;
    else if (index == size) return 0;
}

void pacmanBecomeAHero(Game *game) {
    ghostBecomeDefensive(&game->blinky);
    ghostBecomeDefensive(&game->pinky);
    ghostBecomeDefensive(&game->clyde);
    ghostBecomeDefensive(&game->inky);
}

void ghostBecomeDefensive(Ghost * ghost) {
    ghost->coordinates.speed = GHOST_DEFENSIVE_SPEED;
    ghost->defensiveSecondsLeft = GHOST_MAX_DEFENSIVE_TIME;
}

/// returns sum of the return values of pacmanHitAGhost
void checkPacmanAndGhostsCollision(Game *game) {
    if (areOnTheSamePosition(game->pacman.coordinates.current, game->blinky.coordinates.current) && !game->blinky.defensiveSecondsLeft) {
        pacmanHitAGhost(game, &game->blinky);
    }
    if (areOnTheSamePosition(game->pacman.coordinates.current, game->pinky.coordinates.current) && !game->pinky.defensiveSecondsLeft) {
        pacmanHitAGhost(game, &game->pinky);
    }
    if (areOnTheSamePosition(game->pacman.coordinates.current, game->clyde.coordinates.current) && !game->clyde.defensiveSecondsLeft) {
        pacmanHitAGhost(game, &game->clyde);
    }
    if (areOnTheSamePosition(game->pacman.coordinates.current, game->inky.coordinates.current) && !game->inky.defensiveSecondsLeft) {
        pacmanHitAGhost(game, &game->inky);
    }

    if (areOnTheSamePosition(game->pacman.coordinates.current, game->blinky.coordinates.current) && game->blinky.defensiveSecondsLeft) {
        pacmanHitAGhost(game, &game->blinky);
    }
    if (areOnTheSamePosition(game->pacman.coordinates.current, game->pinky.coordinates.current) && game->pinky.defensiveSecondsLeft) {
        pacmanHitAGhost(game, &game->pinky);
    }
    if (areOnTheSamePosition(game->pacman.coordinates.current, game->clyde.coordinates.current) && game->clyde.defensiveSecondsLeft) {
        pacmanHitAGhost(game, &game->clyde);
    }
    if (areOnTheSamePosition(game->pacman.coordinates.current, game->inky.coordinates.current) && game->inky.defensiveSecondsLeft) {
        pacmanHitAGhost(game, &game->inky);
    }
}

bool areOnTheSamePosition(Point p1, Point p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

/// returns +10: aggresive ghost
/// returns -1: defensive ghost
int pacmanHitAGhost(Game *game, Ghost *ghost) {
    if (ghost->defensiveSecondsLeft) {
        game->pacman.score.ghostsKilled++;
        game->pacman.score.totalScore += GHOST_EAT_VALUE;
        ghost->coordinates.current = ghost->coordinates.start;
        ghost->defensiveSecondsLeft = -1 * DELAY_MADE_WHEN_PACMAN_KILLS_A_GHOST;
        ghost->coordinates.speed = GHOST_AGGRESIVE_SPEED;
        return -1;
    } else {
        restartRoomByPacmanDeath(game);
        return 10;
    }
}

void restartRoomByPacmanDeath(Game *game) {
    game->pacman.hearts--;
    game->pacman.coordinates.current = game->pacman.coordinates.start;
    game->pacman.coordinates.speed = PACMAN_NORMAL_SPEED;
    restartAGhostByPacmanDeath(&game->blinky);
    restartAGhostByPacmanDeath(&game->pinky);
    restartAGhostByPacmanDeath(&game->clyde);
    restartAGhostByPacmanDeath(&game->inky);
}

void restartAGhostByPacmanDeath(Ghost *ghost) {
    ghost->coordinates.speed = GHOST_AGGRESIVE_SPEED;
    ghost->coordinates.current = ghost->coordinates.start;
    ghost->defensiveSecondsLeft = 0;
}

void makeANewRoom(Game *game) {

}

void gameReport(Game *game) {
    char out[game->room.n][game->room.m];
    for (int i = 0; i < game->room.n; ++i)
        for (int j = 0; j < game->room.m; ++j)
            out[i][j] = game->room.tiles[i][j];
    out[game->pacman.coordinates.current.x][game->pacman.coordinates.current.y] = 'C';
    out[game->blinky.coordinates.current.x][game->blinky.coordinates.current.y] = 'B';
    out[game->pinky.coordinates.current.x][game->pinky.coordinates.current.y] = 'P';
    out[game->clyde.coordinates.current.x][game->clyde.coordinates.current.y] = 'L';
    out[game->inky.coordinates.current.x][game->inky.coordinates.current.y] = 'I';
    for (int i = 0; i < game->room.n; ++i) {
        for (int j = 0; j < game->room.m; ++j) {
            char *s = NULL;
            switch (out[i][j]) {
                case '_': s = "░"; break;
                case '#': s = "█"; break;
                case '*': s = "▪"; break;
                case '^': s = "♪"; break;
                case 'O': s = "●"; break;
                default: s = "aoeu"; break;
            }
            if (s != "aoeu")
                printf("%s\t", s);
            else printf("%c\t", out[i][j]);
        }
        printf("\n");
    }
    printf("\n2, 4, 6, 8\n");
}


///////////////////
/// methods end ///
///////////////////

#pragma clang diagnostic pop