#ifndef VIEW_H
#define VIEW_H

#include "models.h"

SDL_Renderer* initiateWindow();

void renderTexture(SDL_Texture*, SDL_Renderer*, int, int, int, int);

SDL_Texture* loadTexture(const char*, SDL_Renderer*);

void paintStage(Game*, SDL_Renderer*);

void renderACharacter(char*, SDL_Texture*, SDL_Renderer*, Coordinates*, int);

void gameReport(Game*);

#endif
