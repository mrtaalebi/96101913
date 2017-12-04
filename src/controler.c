#include <stdlib.h>

#include "controler.h"
#include "input.h"

int runACycle(Game *game) {
    int arrowKeyPressed = listener();
    game->pacman.coordinates.direction = arrowKeyPressed;
    game->stage.cycles++;
    moveACreature(&game->pacman.coordinates, &game->stage);
    pacmanEat(game);
    checkPacmanAndGhostsCollision(game);
    runAGhostACycle(game, &game->blinky);
    runAGhostACycle(game, &game->pinky);
    runAGhostACycle(game, &game->clyde);
    runAGhostACycle(game, &game->inky);
    int foodsLeft = checkRemainingFoods(&game->stage);
    if (foodsLeft == 0 && game->pacman.hearts > 0) {
        return NEW_STAGE;
    } else if (game->pacman.hearts == 0) {
        return GAME_OVER;
    } else {
        return CONTINUE;
    }
}

int runAGhostACycle(Game *game, Ghost *ghost) {
    if (ghost->defensiveSecondsLeft > 0) {
        ghost->defensiveSecondsLeft--;
    }
    Coordinates newOne = ghost->coordinates;
    newOne.direction = rand() % 4 + 1;
    moveACreature(&newOne, &game->stage);
    for (int i = 0; i < 4; ++i) {
        if (areOnTheSamePosition(newOne.current, ghost->coordinates.current)) {
            newOne.direction = (newOne.direction + 1) % 4 + 1;
            moveACreature(&newOne, &game->stage);
        } else break;
    }
    ghost->coordinates = newOne;
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

void moveACreature(Coordinates *p, Stage *room) {
    Point newPoint = p->current;
    switch (p->direction) {
        case DIR_UP:
            newPoint.x = getNextInCircular(newPoint.x - 1, room->n);
            break;
        case DIR_RIGHT:
            newPoint.y = getNextInCircular(newPoint.y + 1, room->m);
            break;
        case DIR_DOWN:
            newPoint.x = getNextInCircular(newPoint.x + 1, room->n);
            break;
        case DIR_LEFT:
            newPoint.y = getNextInCircular(newPoint.y - 1, room->m);
            break;
    }
    char nextStep = room->tiles[newPoint.x][newPoint.y];
    if (nextStep != BLOCK) {
        p->current = newPoint;
    }
}

void pacmanEat(Game *game) {
    char pacmanOn = game->stage.tiles[game->pacman.coordinates.current.x][game->pacman.coordinates.current.y];
    switch (pacmanOn) {
        case CHEESE:
            game->stage.tiles[game->pacman.coordinates.current.x][game->pacman.coordinates.current.y] = EMPTY;
            game->pacman.score.cheeseCount++;
            game->pacman.score.totalScore += CHEESE_VALUE;
            break;
        case CHERRY:
            game->stage.tiles[game->pacman.coordinates.current.x][game->pacman.coordinates.current.y] = EMPTY;
            game->pacman.score.cherryCount++;
            game->pacman.score.totalScore += CHERRY_VALUE;
            break;
        case PINEAPPLE:
            game->stage.tiles[game->pacman.coordinates.current.x][game->pacman.coordinates.current.y] = EMPTY;
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

/// returns +10: aggressive ghost
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
    int roomNumber = game->stage.roomNumber;
    initialize(game);
    game->stage.roomNumber = roomNumber + 1;
}
