#include <stdlib.h>

#include "controler.h"
#include "view.h"
#include "input.h"
#include "models.h"

int runACycle(Game *game) {
    repairBackgrounds(&game->pacman.coordinates, &game->stage);
    if (game->blinky.defensiveCyclesLeft >= 0) repairBackgrounds(&game->blinky.coordinates, &game->stage);
    if (game->pinky.defensiveCyclesLeft >= 0) repairBackgrounds(&game->pinky.coordinates, &game->stage);
    if (game->clyde.defensiveCyclesLeft >= 0) repairBackgrounds(&game->clyde.coordinates, &game->stage);
    if (game->inky.defensiveCyclesLeft >= 0) repairBackgrounds(&game->inky.coordinates, &game->stage);
    game->stage.cycles++;
    pacmanEat(game);
    checkPacmanAndGhostsCollision(game);
    runAGhostACycle(game, &game->blinky);
    runAGhostACycle(game, &game->pinky);
    runAGhostACycle(game, &game->clyde);
    runAGhostACycle(game, &game->inky);
    if (++game->pacman.coordinates.waitedCycles == game->pacman.coordinates.cyclesPerMove) {
        game->pacman.coordinates.waitedCycles = 0;
        int arrowKeyPressed = listener();
        decideNextDirection(&game->pacman.coordinates, &game->stage, arrowKeyPressed);
        moveACharacter(&game->pacman.coordinates, &game->stage);
        paintCharacter(&game->pacman.coordinates);
    }
    paintCharacter(&game->pacman.coordinates);
    if (game->blinky.defensiveCyclesLeft >= 0) paintCharacter(&game->blinky.coordinates);
    if (game->pinky.defensiveCyclesLeft >= 0) paintCharacter(&game->pinky.coordinates);
    if (game->clyde.defensiveCyclesLeft >= 0) paintCharacter(&game->clyde.coordinates);
    if (game->inky.defensiveCyclesLeft >= 0) paintCharacter(&game->inky.coordinates);
    renderPresent();
    int foodsLeft = checkRemainingFoods(&game->stage);
    if (foodsLeft == 0 && game->pacman.hearts > 0) {
        return NEW_STAGE;
    } else if (game->pacman.hearts == 0) {
        return GAME_OVER;
    } else {
        return CONTINUE;
    }
}

void makeMoveForAGhost(Ghost* ghost, Stage* stage) {
    if (ghost->defensiveCyclesLeft >= 0) {
        repairBackgrounds(&ghost->coordinates, stage);
        paintCharacter(&ghost->coordinates);
    }
}

/// get your fucking hands off this function
/// changing a single bit will cause bugs...
/// so never ever think about changing it :)
void moveACharacter(Coordinates *coordinates, Stage *stage) {
    Point newPoint = coordinates->currentPosition;
    bool moveOkay = false;
    switch (coordinates->direction) {
        case DIR_UP:
            newPoint.x = getNextInCircular(newPoint.x - 1, stage->n);
            if (stage->tiles[newPoint.x / TILE][newPoint.y / TILE] != BLOCK &&
                stage->tiles[newPoint.x / TILE][(newPoint.y + TILE - 1) / TILE] != BLOCK) {
                moveOkay = true;
            }
            break;
        case DIR_RIGHT:
            newPoint.y = getNextInCircular(newPoint.y + 1, stage->m);
            if (stage->tiles[newPoint.x / TILE][(newPoint.y + TILE - 1) / TILE] != BLOCK &&
                stage->tiles[(newPoint.x + TILE - 1) / TILE][(newPoint.y + TILE - 1) / TILE] != BLOCK) {
                moveOkay = true;
            }
            break;
        case DIR_DOWN:
            newPoint.x = getNextInCircular(newPoint.x + 1, stage->n);
            if (stage->tiles[(newPoint.x + TILE - 1) / TILE][newPoint.y / TILE] != BLOCK &&
                stage->tiles[(newPoint.x + TILE - 1) / TILE][(newPoint.y + TILE - 1) / TILE] != BLOCK) {
                moveOkay = true;
            }
            break;
        case DIR_LEFT:
            newPoint.y = getNextInCircular(newPoint.y - 1, stage->m);
            if (stage->tiles[newPoint.x / TILE][newPoint.y / TILE] != BLOCK &&
                stage->tiles[(newPoint.x + TILE - 1) / TILE][newPoint.y / TILE] != BLOCK) {
                moveOkay = true;
            }
            break;
        default:
            break;
    }
    if (moveOkay) {
        coordinates->currentPosition = newPoint;
    }
}

bool areOnTheSameExactPosition(Point p1, Point p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

/// this could be touched
int getNextInCircular(int index, int size) {
    size *= TILE;
    if (index < 0) return  size + index;
    else if (index >= 0 && index < size) return index;
    else if (index >= size) return index - size;

    // todo a small visual bug can be fixed but not a stopping bug!
}

void repairBackgrounds(Coordinates* coordinates, Stage* stage) {
    int exactX = coordinates->currentPosition.x / TILE;
    int exactY = coordinates->currentPosition.y / TILE;
    switch (coordinates->direction) {
        case DIR_UP:
            paintBackgrounds(exactX, exactY, stage->tiles[exactX][exactY]);
            if (coordinates->currentPosition.x % TILE != 0)
                paintBackgrounds(exactX + 1, exactY, stage->tiles[exactX + 1][exactY]);
            break;
        case DIR_RIGHT:
            paintBackgrounds(exactX, exactY, stage->tiles[exactX][exactY]);
            if (coordinates->currentPosition.y % TILE != 0)
                paintBackgrounds(exactX, exactY + 1, stage->tiles[exactX][exactY + 1]);
            break;
        case DIR_DOWN:
            paintBackgrounds(exactX, exactY, stage->tiles[exactX][exactY]);
            if (coordinates->currentPosition.x % TILE != 0)
                paintBackgrounds(exactX + 1, exactY, stage->tiles[exactX + 1][exactY]);
            break;
        case DIR_LEFT:
            paintBackgrounds(exactX, exactY, stage->tiles[exactX][exactY]);
            if (coordinates->currentPosition.y % TILE != 0)
                paintBackgrounds(exactX, exactY + 1, stage->tiles[exactX][exactY + 1]);
            break;
        default:
            break;
    }
}

/// this shouldn't get touched because me myself don't know how it works!
void decideNextDirection(Coordinates *coordinates, Stage *stage, int newDirection) {
    if (newDirection != DIR_NONE) {
        Coordinates newCoordinates = *coordinates;
        newCoordinates.direction = newDirection;
        moveACharacter(&newCoordinates, stage);
        if (areOnTheSameExactPosition(newCoordinates.currentPosition, coordinates->currentPosition)) {
            coordinates->nextDirection = newDirection;
        } else {
            coordinates->direction = newDirection;
        }
    }
    reallyChangeDirection(coordinates, stage);
}

/// this is like previous too
void reallyChangeDirection(Coordinates *coordinates, Stage *stage) {
    if (coordinates->nextDirection != DIR_NONE) {
        Coordinates newCoordinates = *coordinates;
        newCoordinates.direction = coordinates->nextDirection;
        moveACharacter(&newCoordinates, stage);
        if (!areOnTheSameExactPosition(newCoordinates.currentPosition, coordinates->currentPosition)) {
            coordinates->direction = coordinates->nextDirection;
            coordinates->nextDirection = DIR_NONE;
        }
    }
}

int runAGhostACycle(Game *game, Ghost *ghost) {
    if (ghost->defensiveCyclesLeft > 0) {
        ghost->defensiveCyclesLeft--;
        ghost->coordinates.isDefensive = true;
        ghost->coordinates.cyclesPerMove = GHOST_DEFENSIVE_CYCLES_PER_MOVE;
    } else if (ghost->defensiveCyclesLeft == 0) {
        ghost->coordinates.cyclesPerMove = GHOST_AGGRESSIVE_CYCLES_PER_MOVE;
        ghost->coordinates.isDefensive = false;
    } else {
        ghost->defensiveCyclesLeft++;
        return 0;
    }
    if (++ghost->coordinates.waitedCycles >= ghost->coordinates.cyclesPerMove) {
        ghost->coordinates.waitedCycles = 0;
        Coordinates newOne = ghost->coordinates;
        moveACharacter(&newOne, &game->stage);
        if (areOnTheSameExactPosition(newOne.currentPosition, ghost->coordinates.currentPosition)) {
            newOne.direction = rand() % 4 + 1;
            moveACharacter(&newOne, &game->stage);
            for (int i = 0; i < 4; ++i) {
                if (areOnTheSameExactPosition(newOne.currentPosition, ghost->coordinates.currentPosition)) {
                    newOne.direction = (newOne.direction + 1) % 4 + 1;
                    moveACharacter(&newOne, &game->stage);
                } else break;
            }
        }
        ghost->coordinates.direction = newOne.direction;
        moveACharacter(&ghost->coordinates, &game->stage);
    }
}

int checkRemainingFoods(Stage *stage) {
    return stage->numberOfFoods;
}

void pacmanEat(Game *game) {
    int middleX = getNextInCircular(game->pacman.coordinates.currentPosition.x + TILE / 2, game->stage.n);
    int middleY = getNextInCircular(game->pacman.coordinates.currentPosition.y + TILE / 2, game->stage.m);
    int exactX = getNextInCircular(middleX - middleX % TILE, game->stage.n);
    int exactY = getNextInCircular(middleY - middleY % TILE, game->stage.m);
    char pacmanOn = game->stage.tiles[exactX / TILE][exactY / TILE];
    bool hasAte = false;
    switch (pacmanOn) {
        case CHEESE:
            hasAte = true;
            game->pacman.score.cheeseCount++;
            game->pacman.score.totalScore += CHEESE_VALUE;
            game->stage.numberOfFoods--;
            break;
        case CHERRY:
            hasAte = true;
            game->pacman.score.cherryCount++;
            game->pacman.score.totalScore += CHERRY_VALUE;
            break;
        case PINEAPPLE:
            hasAte = true;
            game->pacman.score.pineappleCount++;
            pacmanBecomeAHero(game);
            game->stage.numberOfFoods--;
            break;
        default:
            break;
    }
    if (hasAte) {
        emptySomeBackgroundCells(exactX, exactY, &game->stage);
        paintBackgrounds(exactX / TILE, exactY / TILE, EMPTY);
    }
}

void emptySomeBackgroundCells(int startX, int startY, Stage* stage) {
    stage->tiles[startX / TILE][startY / TILE] = EMPTY;
}

void pacmanBecomeAHero(Game *game) {
    ghostBecomeDefensive(&game->blinky);
    ghostBecomeDefensive(&game->pinky);
    ghostBecomeDefensive(&game->clyde);
    ghostBecomeDefensive(&game->inky);
}

void ghostBecomeDefensive(Ghost * ghost) {
    ghost->coordinates.cyclesPerMove = GHOST_DEFENSIVE_CYCLES_PER_MOVE;
    ghost->defensiveCyclesLeft = GHOST_MAX_DEFENSIVE_TIME_SECONDS * CYCLES_PER_SECOND;
}

void checkPacmanAndGhostsCollision(Game *game) {
    if (areNearEnoughToStrike(game->pacman.coordinates.currentPosition, game->blinky.coordinates.currentPosition) && !game->blinky.defensiveCyclesLeft)
        pacmanHitAGhost(game, &game->blinky);
    if (areNearEnoughToStrike(game->pacman.coordinates.currentPosition, game->pinky.coordinates.currentPosition) && !game->pinky.defensiveCyclesLeft)
        pacmanHitAGhost(game, &game->pinky);
    if (areNearEnoughToStrike(game->pacman.coordinates.currentPosition, game->clyde.coordinates.currentPosition) && !game->clyde.defensiveCyclesLeft)
        pacmanHitAGhost(game, &game->clyde);
    if (areNearEnoughToStrike(game->pacman.coordinates.currentPosition, game->inky.coordinates.currentPosition) && !game->inky.defensiveCyclesLeft)
        pacmanHitAGhost(game, &game->inky);
    if (areNearEnoughToStrike(game->pacman.coordinates.currentPosition, game->blinky.coordinates.currentPosition) && game->blinky.defensiveCyclesLeft)
        pacmanHitAGhost(game, &game->blinky);
    if (areNearEnoughToStrike(game->pacman.coordinates.currentPosition, game->pinky.coordinates.currentPosition) && game->pinky.defensiveCyclesLeft)
        pacmanHitAGhost(game, &game->pinky);
    if (areNearEnoughToStrike(game->pacman.coordinates.currentPosition, game->clyde.coordinates.currentPosition) && game->clyde.defensiveCyclesLeft)
        pacmanHitAGhost(game, &game->clyde);
    if (areNearEnoughToStrike(game->pacman.coordinates.currentPosition, game->inky.coordinates.currentPosition) && game->inky.defensiveCyclesLeft)
        pacmanHitAGhost(game, &game->inky);
}

bool areNearEnoughToStrike(Point p1, Point p2) {
    return ((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)) < ((float) TILE * TILE / 4.0);
}


/// returns +10: aggressive ghost
/// returns -1: defensive ghost
int pacmanHitAGhost(Game *game, Ghost *ghost) {
    if (ghost->defensiveCyclesLeft) {
        repairBackgrounds(&ghost->coordinates, &game->stage);
        game->pacman.score.ghostsKilled++;
        game->pacman.score.totalScore += GHOST_EAT_VALUE;
        ghost->coordinates.currentPosition = ghost->coordinates.startPosition;
        ghost->defensiveCyclesLeft = -1 * DELAY_MADE_WHEN_PACMAN_KILLS_A_GHOST * CYCLES_PER_SECOND;
        ghost->coordinates.cyclesPerMove = GHOST_DEFENSIVE_CYCLES_PER_MOVE;
        ghost->coordinates.waitedCycles = 0;
        return -1;
    } else {
        restartRoomByPacmanDeath(game);
        return 10;
    }
}

void restartRoomByPacmanDeath(Game *game) {
    SDL_Delay(500);
    repairBackgrounds(&game->pacman.coordinates, &game->stage);
    game->pacman.hearts--;
    game->pacman.coordinates.currentPosition = game->pacman.coordinates.startPosition;
    game->pacman.coordinates.direction = DIR_NONE;
    game->pacman.coordinates.cyclesPerMove = PACMAN_CYCLES_PER_MOVE;
    game->pacman.coordinates.waitedCycles = 0;
    restartAGhostByPacmanDeath(&game->blinky, &game->stage);
    restartAGhostByPacmanDeath(&game->pinky, &game->stage);
    restartAGhostByPacmanDeath(&game->clyde, &game->stage);
    restartAGhostByPacmanDeath(&game->inky, &game->stage);
}

void restartAGhostByPacmanDeath(Ghost *ghost, Stage* stage) {
    repairBackgrounds(&ghost->coordinates, stage);
    ghost->coordinates.cyclesPerMove = GHOST_AGGRESSIVE_CYCLES_PER_MOVE;
    ghost->coordinates.waitedCycles = 0;
    ghost->coordinates.direction = DIR_NONE;
    ghost->coordinates.currentPosition = ghost->coordinates.startPosition;
    ghost->defensiveCyclesLeft = 0;
}

void makeANewRoom(Game *game) {
    int roomNumber = game->stage.roomNumber;
    int pacmanHearts = game->pacman.hearts;
    initialize(game);
    game->pacman.hearts = pacmanHearts;
    game->stage.roomNumber = roomNumber + 1;
    SDL_Delay(500);
}
