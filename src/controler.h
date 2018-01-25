#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdbool.h>
#include "models.h"

int runACycle(Game *, Direction);

int runAGhostACycle(Game *, Ghost *);

int mod(int a, int b);

int distance(Point p1, Point p2);

Point destination(CharacterType ghostType, Coordinates* blinky, Coordinates* pacman, Coordinates* me, Stage* stage);

void makeANewRoom(Game *);

void moveACharacter(Coordinates *, Stage *);

void runPacmanACycle(Game*, Direction);

void reallyChangeDirection(Coordinates *, Stage *);

void decideNextDirection(Coordinates *, Stage *, int);

void pacmanBecomeAHero(Game *);

int pacmanHitAGhost(Game *, Ghost *);

void checkPacmanAndGhostsCollision(Game *);

bool areNearEnoughToStrike(Point, Point);

bool areOnTheSameExactPosition(Point, Point);

int checkRemainingFoods(Stage *);

void ghostBecomeDefensive(Ghost *);

void restartRoomByPacmanDeath(Game *);

void restartAGhostByPacmanDeath(Ghost *);

int getNextInCircular(int, int);

void pacmanEat(Game *);

Direction shortestPath(Stage* stage, Point on, Point to);

void emptySomeBackgroundCells(int, int, Stage*);

#endif
