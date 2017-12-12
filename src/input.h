#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>

#include "models.h"

enum {
    MOUSE_EVENT, ARROW_KEY_EVENT, WINDOW_EVENT
} typedef EVENT_TYPE;

int listener(int eventType);

void initialize(Game*);

void pacmanInit(Pacman*, FILE*);

void ghostInit(Ghost*, FILE*, int characterType);

void characterCoordinatesToPixels(Coordinates *);

#endif
