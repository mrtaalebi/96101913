#ifndef UTILS_H
#define UTILS_H

#include <SDL_pixels.h>

struct name {
    char* string;
    int x;
    int y;
    int cyclesRemaining;
    SDL_Color color;
    struct name* next;
} typedef Name;

#endif
