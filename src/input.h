#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>

#include "models.h"
#include "gui.h"

int listener();

void initialize(Game*);

void pacmanInit(Pacman*, FILE*);

void ghostInit(Ghost*, FILE*, int characterType);

void characterCoordinatesToPixels(Coordinates *);

Fame* readFames();

void writeFames(Fame);

#endif
