#ifndef VIEW_H
#define VIEW_H

#include "models.h"

SDL_Renderer* initiateWindow();

void renderTexture(SDL_Texture*, SDL_Renderer*, int, int, int, int);

SDL_Texture* loadTexture(const char*, SDL_Renderer*) ;

void paintStage(Game*, SDL_Renderer*);

void renderACharacter(char *file, SDL_Texture *icon, SDL_Renderer *ren, Coordinates *coordinates, int TILE) ;

void gameReport(Game* game);

#endif