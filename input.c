#include <stdio.h>

#include "input.h"


int listener() {
    int direction;
    direction = getchar() - '0';
    switch (direction) {
        case 8:
            direction = 1;
            break;
        case 6:
            direction = 2;
            break;
        case 2:
            direction = 3;
            break;
        case 4:
            direction = 4;
            break;
        default:
            return -1;
            break;
    }
    printf("dir: %d\n", direction);
    return direction;
}