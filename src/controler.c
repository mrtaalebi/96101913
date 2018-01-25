#include <stdlib.h>

#include "controler.h"
#include "view.h"
#include "input.h"
#include "models.h"

int runACycle(Game *game, Direction pacmanDirection) {
    game->stage.cycles++;
    paintStage(game);
    runAGhostACycle(game, &game->blinky);
    runAGhostACycle(game, &game->pinky);
    runAGhostACycle(game, &game->clyde);
    runAGhostACycle(game, &game->inky);
    checkPacmanAndGhostsCollision(game);
    runPacmanACycle(game, pacmanDirection);
    if (checkRemainingFoods(&game->stage) == 0 && game->pacman.hearts > 0) return NEW_STAGE;
    else if (game->pacman.hearts == 0) return GAME_OVER;
    else return CONTINUE;
}

void runPacmanACycle(Game* game, Direction arrowKey) {
    pacmanEat(game);
    if (++game->pacman.coordinates.waitedCycles == game->pacman.coordinates.cyclesPerMove) {
        game->pacman.coordinates.waitedCycles = 0;
        decideNextDirection(&game->pacman.coordinates, &game->stage, arrowKey);
        moveACharacter(&game->pacman.coordinates, &game->stage);
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
    if (ghost->defensiveCyclesLeft == 0) {
        ghost->coordinates.cyclesPerMove = GHOST_AGGRESSIVE_CYCLES_PER_MOVE;
        ghost->coordinates.isDefensive = false;
    } else if (ghost->defensiveCyclesLeft > 0) {
        ghost->defensiveCyclesLeft--;
        ghost->coordinates.isDefensive = true;
        ghost->coordinates.cyclesPerMove = GHOST_DEFENSIVE_CYCLES_PER_MOVE;
    } else {
        ghost->coordinates.cyclesPerMove = GHOST_AGGRESSIVE_CYCLES_PER_MOVE;
        if (areOnTheSameExactPosition(ghost->coordinates.currentPosition, ghost->coordinates.startPosition))
            ghost->defensiveCyclesLeft = 0;
    }

    if (ghost->coordinates.waitedCycles == ghost->coordinates.cyclesPerMove) {
        ghost->coordinates.waitedCycles = 1;
        Point intOn = {ghost->coordinates.currentPosition.x / TILE, ghost->coordinates.currentPosition.y / TILE};
        Point intTo;
        if (ghost->defensiveCyclesLeft > 0) {
            intTo.x = getNextInCircular(2 * ghost->coordinates.startPosition.x - game->pacman.coordinates.currentPosition.x, game->stage.n);
            intTo.y = getNextInCircular(2 * ghost->coordinates.startPosition.y - game->pacman.coordinates.currentPosition.y, game->stage.m);
        } else if (ghost->defensiveCyclesLeft < 0) {
            intTo.x = ghost->coordinates.startPosition.x;
            intTo.y = ghost->coordinates.startPosition.y;
        } else {
            intTo = destination(ghost->coordinates.characterType, &game->blinky.coordinates, &game->pacman.coordinates, &ghost->coordinates, &game->stage);
        }
        intTo.x /= TILE;
        intTo.y /= TILE;
        Coordinates test = ghost->coordinates;
        test.direction = shortestPath(&game->stage, intOn, intTo);
        moveACharacter(&test, &game->stage);
        if (!areOnTheSameExactPosition(ghost->coordinates.currentPosition, test.currentPosition))
            ghost->coordinates.direction = test.direction;
        moveACharacter(&ghost->coordinates, &game->stage);
    } else if (ghost->coordinates.waitedCycles > ghost->coordinates.cyclesPerMove)
        ghost->coordinates.waitedCycles = 0;
    else
        ghost->coordinates.waitedCycles++;
}

Point destination(CharacterType ghostType, Coordinates* blinky, Coordinates* pacman, Coordinates* me, Stage* stage) {
    int dx[6] = {0, -TILE, 0, TILE, 0, 0};
    int dy[6] = {0, 0, -TILE, 0, TILE, 0};
    //return pacman->currentPosition;
    Point result;
    switch (ghostType) {
        case CHARACTER_BLINKY:
            return pacman->currentPosition;
        case CHARACTER_PINKY:
            result = pacman->currentPosition;
            result.x = getNextInCircular(result.x + 4 * dx[pacman->direction], stage->n);
            result.y = getNextInCircular(result.y + 4 * dy[pacman->direction], stage->m);
            while (!areOnTheSameExactPosition(result, pacman->currentPosition)) {
                Point wayToPacman = result;
                bool free = true;
                while (!areOnTheSameExactPosition(wayToPacman, pacman->currentPosition)) {
                    if (stage->tiles[wayToPacman.x / TILE][wayToPacman.y / TILE] == BLOCK) {
                        free = false;
                        break;
                    }
                    wayToPacman.x = getNextInCircular(wayToPacman.x - dx[pacman->direction], stage->n);
                    wayToPacman.y = getNextInCircular(wayToPacman.y - dy[pacman->direction], stage->m);
                }
                if (free) {
                    return result;
                }
                result.x = getNextInCircular(result.x - dx[pacman->direction], stage->n);
                result.y = getNextInCircular(result.y - dy[pacman->direction], stage->m);
            }
            return result;
        case CHARACTER_CLYDE:
            result = pacman->currentPosition;
            if (abs(result.x - me->currentPosition.x) + abs(result.y - me->currentPosition.y) > TILE * 12) {
                return result;
            }
            result.x = TILE * 28;
            result.y = TILE * 5;
            return result;
        case CHARACTER_INKY:
            result = pacman->currentPosition;
            result.x = getNextInCircular(2 * (pacman->currentPosition.x + 2 * dx[pacman->direction]) - blinky->currentPosition.x, stage->n);
            result.y = getNextInCircular(2 * (pacman->currentPosition.y + 2 * dy[pacman->direction]) - blinky->currentPosition.y, stage->m);
            return result;
        default: return pacman->currentPosition;
    }

}

/// to programmer: only accepts TILE values for x and y
Direction shortestPath(Stage* stage, Point on, Point to) {
    int n = stage->n;
    int m = stage->m;
    Point* queue = malloc(sizeof(Point) * n * m);
    int front = 0;
    int back = 1;
    Direction way[n][m];
    bool visited[n][m];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            visited[i][j] = stage->tiles[i][j] == BLOCK;
            way[i][j] = DIR_NONE;
        }
    }
    visited[on.x][on.y] = true;
    way[mod(on.x - 1, n)][on.y] = DIR_UP;
    way[mod(on.x + 1, n)][on.y] = DIR_DOWN;
    way[on.x][mod(on.y - 1, m)] = DIR_LEFT;
    way[on.x][mod(on.y + 1, m)] = DIR_RIGHT;
    queue[0] = on;
    while (back > front) {
        // success condition
        if (areOnTheSameExactPosition(queue[front], to)) {
            break;
        }
        // removing
        Point parent = queue[front];
        visited[parent.x][parent.y] = true;
        front++;

        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i * j != 0) continue;
                if (!visited[mod(parent.x + i, n)][mod(parent.y + j, m)]) {
                    Point new = {mod(parent.x + i, n), mod(parent.y + j, m)};
                    queue[back++] = new;
                    if (front > 1) {
                        int a = mod(parent.x + i, n);
                        int b = mod(parent.y + j, m);
                        way[a][b] = way[parent.x][parent.y];
                    }
                }
            }
        }
    }
    return way[queue[front].x][queue[front].y];
}

int mod(int a, int b) {
    if (a == -1) return b - 1;
    if (a == b) return 0;
    return a;
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
    int value = 0;
    switch (pacmanOn) {
        case CHEESE:
            hasAte = true;
            game->pacman.score.cheeseCount++;
            game->pacman.score.totalScore += CHEESE_VALUE;
            value = CHEESE_VALUE;
            game->stage.numberOfFoods--;
            break;
        case CHERRY:
            hasAte = true;
            game->pacman.score.cherryCount++;
            game->pacman.score.totalScore += CHERRY_VALUE;
            value = CHERRY_VALUE;
            break;
        case PINEAPPLE:
            hasAte = true;
            game->pacman.score.pineappleCount++;
            game->pacman.score.totalScore += PINEAPPLE_VALUE;
            value = PINEAPPLE_VALUE;
            pacmanBecomeAHero(game);
            game->stage.numberOfFoods--;
            break;
        default:
            break;
    }
    if (hasAte) {
        emptySomeBackgroundCells(exactX, exactY, &game->stage);
        char* score = malloc(20);
        sprintf(score, "%d", value);
        drawText(score, exactY, exactX, COLOR_EAT_GOODS);
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
    if (ghost->defensiveCyclesLeft >= 0)
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
    return distance(p1, p2) < (TILE * 1.5);
}

int distance(Point p1, Point p2) {
    return (int) sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}


/// returns +10: aggressive ghost
/// returns -1: defensive ghost
int pacmanHitAGhost(Game *game, Ghost *ghost) {
    if (ghost->defensiveCyclesLeft > 0) {
        char* string = malloc(20);
        sprintf(string, "%d", GHOST_EAT_VALUE);
        drawText(string, ghost->coordinates.currentPosition.y, ghost->coordinates.currentPosition.x, COLOR_EAT_GHOST);
        game->pacman.score.ghostsKilled++;
        game->pacman.score.totalScore += GHOST_EAT_VALUE;
        ghost->defensiveCyclesLeft = -1;
        ghost->coordinates.cyclesPerMove = GHOST_DEFENSIVE_CYCLES_PER_MOVE;
        ghost->coordinates.waitedCycles = 0;
        return -1;
    } else if (ghost->defensiveCyclesLeft == 0){
        restartRoomByPacmanDeath(game);
        return 10;
    }
}

void restartRoomByPacmanDeath(Game *game) {
    SDL_Delay(500);
    game->pacman.hearts--;
    game->pacman.coordinates.currentPosition = game->pacman.coordinates.startPosition;
    game->pacman.coordinates.direction = DIR_NONE;
    game->pacman.coordinates.cyclesPerMove = PACMAN_CYCLES_PER_MOVE;
    game->pacman.coordinates.waitedCycles = 0;
    restartAGhostByPacmanDeath(&game->blinky);
    restartAGhostByPacmanDeath(&game->pinky);
    restartAGhostByPacmanDeath(&game->clyde);
    restartAGhostByPacmanDeath(&game->inky);
}

void restartAGhostByPacmanDeath(Ghost *ghost) {
    ghost->coordinates.cyclesPerMove = GHOST_AGGRESSIVE_CYCLES_PER_MOVE;
    ghost->coordinates.waitedCycles = 0;
    ghost->coordinates.direction = DIR_NONE;
    ghost->coordinates.currentPosition = ghost->coordinates.startPosition;
    ghost->defensiveCyclesLeft = 0;
}

void makeANewRoom(Game *game) {
    int roomNumber = game->stage.roomNumber;
    int cycles = game->stage.cycles;
    int pacmanHearts = game->pacman.hearts;
    Score pacmanScore = game->pacman.score;
    initialize(game);
    game->pacman.score = pacmanScore;
    game->pacman.hearts = pacmanHearts;
    game->stage.roomNumber = roomNumber + 1;
    game->stage.cycles = cycles;
    SDL_Delay(500);
}
