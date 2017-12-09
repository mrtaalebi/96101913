#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdbool.h>
#include "models.h"

int runACycle(Game *);

int runAGhostACycle(Game *, Ghost *);

void makeANewRoom(Game *);

void moveACharacter(Coordinates *, Stage *, bool);

void repairBackgrounds(Coordinates*, Stage*);

void reallyChangeDirection(Coordinates *, Stage *);

void decideNextDirection(Coordinates *, Stage *, int);

void pacmanBecomeAHero(Game *);

int pacmanHitAGhost(Game *, Ghost *);

void checkPacmanAndGhostsCollision(Game *);

bool areNearEnoughToStrike(Point, Point, Game*);

bool areOnTheSameExactPosition(Point, Point);

int checkRemainingFoods(Stage *);

void ghostBecomeDefensive(Ghost *);

void restartRoomByPacmanDeath(Game *);

void restartAGhostByPacmanDeath(Ghost *, Stage*);

int getNextInCircular(int, int);

void pacmanEat(Game *);

void emptySomeBackgroundCells(int, int, Stage*);

#endif
