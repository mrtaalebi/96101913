#ifndef VIEW_H
#define VIEW_H

#include <SDL2/SDL.h>

#include "models.h"

void initiateWindow();

void renderTexture(SDL_Texture*, int, int, int, int);

SDL_Texture* loadTexture(const char*);

void paintStage(Game*);

void renderACharacter(Coordinates*);

void paintBackgrounds(int, int, char);

void paintCharacterWhenMoved(Coordinates, Point*, Stage*);

void cleanACorpse(Coordinates *, Stage *);

void gameReport(Game*);

#endif
