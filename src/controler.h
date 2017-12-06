#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdbool.h>
#include "models.h"

int runACycle(Game *);

int runAGhostACycle(Game *, Ghost *);

void makeANewRoom(Game *);

void moveACreature(Coordinates *, Stage *);

void pacmanDirectionDecide(Coordinates *, Stage *, int);

void pacmanBecomeAHero(Game *);

int pacmanHitAGhost(Game *, Ghost *);

void checkPacmanAndGhostsCollision(Game *);

bool areOnTheSamePosition(Point, Point);

int checkRemainingFoods(Stage *);

void ghostBecomeDefensive(Ghost *);

void restartRoomByPacmanDeath(Game *);

void restartAGhostByPacmanDeath(Ghost *, Stage*);

int getNextInCircular(int, int);

void pacmanEat(Game *);

#endif
