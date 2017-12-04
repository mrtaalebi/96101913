#ifndef CONTROLER_H
#define CONTROLER_H

#include <stdbool.h>
#include "models.h"


int runACycle(Game *);

int runAGhostACycle(Game *game, Ghost *ghost);

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

void pacmanEat(Game *);

#endif