#include <stdlib.h>

#include "controler.h"
#include "input.h"
#include "view.h"
#include "models.h"

int runACycle(Game *game) {
    game->stage.cycles++;
    int arrowKeyPressed = listener();
    if (arrowKeyPressed != -1) {
        pacmanDirectionDecide(&game->pacman.coordinates, &game->stage, arrowKeyPressed);
    }
    moveACreature(&game->pacman.coordinates, &game->stage);
    checkPacmanAndGhostsCollision(game);
    pacmanEat(game);
    runAGhostACycle(game, &game->blinky);
    runAGhostACycle(game, &game->pinky);
    runAGhostACycle(game, &game->clyde);
    runAGhostACycle(game, &game->inky);
    checkPacmanAndGhostsCollision(game);
    int foodsLeft = checkRemainingFoods(&game->stage);
    if (foodsLeft == 0 && game->pacman.hearts > 0) {
        return NEW_STAGE;
    } else if (game->pacman.hearts == 0) {
        return GAME_OVER;
    } else {
        return CONTINUE;
    }
}

void pacmanDirectionDecide(Coordinates *coordinates, Stage *stage, int newDirection) {
    Coordinates newOne = *coordinates;
    newOne.direction = newDirection;
    moveACreature(&newOne, stage);
    if (!areOnTheSamePosition(newOne.current, coordinates->current)) {
        coordinates->direction = newDirection;
    }
}

int runAGhostACycle(Game *game, Ghost *ghost) {
    if (ghost->defensiveCyclesLeft > 0) {
        ghost->defensiveCyclesLeft--;
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
    } else {
        ghost->defensiveCyclesLeft++;
    }
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

void moveACreature(Coordinates *p, Stage *stage) {
    Point newPoint = p->current;
    switch (p->direction) {
        case DIR_UP:
            newPoint.x = getNextInCircular(newPoint.x - 1, stage->n);
            break;
        case DIR_RIGHT:
            newPoint.y = getNextInCircular(newPoint.y + 1, stage->m);
            break;
        case DIR_DOWN:
            newPoint.x = getNextInCircular(newPoint.x + 1, stage->n);
            break;
        case DIR_LEFT:
            newPoint.y = getNextInCircular(newPoint.y - 1, stage->m);
            break;
    }
    char nextStep = stage->tiles[newPoint.x][newPoint.y];
    if (nextStep != BLOCK) {
        paintCharacterWhenMoved(*p, &newPoint, stage);
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
    ghost->defensiveCyclesLeft = GHOST_MAX_DEFENSIVE_TIME_SECONDS * CYCLES_PER_SECOND;
}

void checkPacmanAndGhostsCollision(Game *game) {
    if (areOnTheSamePosition(game->pacman.coordinates.current, game->blinky.coordinates.current) && !game->blinky.defensiveCyclesLeft) {
        pacmanHitAGhost(game, &game->blinky);
    }
    if (areOnTheSamePosition(game->pacman.coordinates.current, game->pinky.coordinates.current) && !game->pinky.defensiveCyclesLeft) {
        pacmanHitAGhost(game, &game->pinky);
    }
    if (areOnTheSamePosition(game->pacman.coordinates.current, game->clyde.coordinates.current) && !game->clyde.defensiveCyclesLeft) {
        pacmanHitAGhost(game, &game->clyde);
    }
    if (areOnTheSamePosition(game->pacman.coordinates.current, game->inky.coordinates.current) && !game->inky.defensiveCyclesLeft) {
        pacmanHitAGhost(game, &game->inky);
    }

    if (areOnTheSamePosition(game->pacman.coordinates.current, game->blinky.coordinates.current) && game->blinky.defensiveCyclesLeft) {
        pacmanHitAGhost(game, &game->blinky);
    }
    if (areOnTheSamePosition(game->pacman.coordinates.current, game->pinky.coordinates.current) && game->pinky.defensiveCyclesLeft) {
        pacmanHitAGhost(game, &game->pinky);
    }
    if (areOnTheSamePosition(game->pacman.coordinates.current, game->clyde.coordinates.current) && game->clyde.defensiveCyclesLeft) {
        pacmanHitAGhost(game, &game->clyde);
    }
    if (areOnTheSamePosition(game->pacman.coordinates.current, game->inky.coordinates.current) && game->inky.defensiveCyclesLeft) {
        pacmanHitAGhost(game, &game->inky);
    }
}

bool areOnTheSamePosition(Point p1, Point p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

/// returns +10: aggressive ghost
/// returns -1: defensive ghost
int pacmanHitAGhost(Game *game, Ghost *ghost) {
    if (ghost->defensiveCyclesLeft) {
        game->pacman.score.ghostsKilled++;
        game->pacman.score.totalScore += GHOST_EAT_VALUE;
        cleanACorpse(&ghost->coordinates, &game->stage);
        ghost->coordinates.current = ghost->coordinates.start;
        ghost->defensiveCyclesLeft = -1 * DELAY_MADE_WHEN_PACMAN_KILLS_A_GHOST * CYCLES_PER_SECOND;
        ghost->coordinates.speed = GHOST_AGGRESSIVE_SPEED;
        return -1;
    } else {
        restartRoomByPacmanDeath(game);
        return 10;
    }
}

void restartRoomByPacmanDeath(Game *game) {
    game->pacman.hearts--;
    cleanACorpse(&game->pacman.coordinates, &game->stage);
    game->pacman.coordinates.current = game->pacman.coordinates.start;
    game->pacman.coordinates.direction = DIR_NONE;
    game->pacman.coordinates.speed = PACMAN_NORMAL_SPEED;
    restartAGhostByPacmanDeath(&game->blinky, &game->stage);
    restartAGhostByPacmanDeath(&game->pinky, &game->stage);
    restartAGhostByPacmanDeath(&game->clyde, &game->stage);
    restartAGhostByPacmanDeath(&game->inky, &game->stage);
}

void restartAGhostByPacmanDeath(Ghost *ghost, Stage* stage) {
    ghost->coordinates.speed = GHOST_AGGRESSIVE_SPEED;
    cleanACorpse(&ghost->coordinates, stage);
    ghost->coordinates.direction = DIR_NONE;
    ghost->coordinates.current = ghost->coordinates.start;
    ghost->defensiveCyclesLeft = 0;
}

void makeANewRoom(Game *game) {
    int roomNumber = game->stage.roomNumber;
    int pacmanHearts = game->pacman.hearts;
    initialize(game);
    game->pacman.hearts = pacmanHearts;
    game->stage.roomNumber = roomNumber + 1;
}
