#ifndef VIEW_H
#define VIEW_H

#include "models.h"

void initiateWindow();

void paintStage(Game*);

void renderACharacter(Coordinates*);

void paintBackgrounds(int, int, char);

void paintCharacter(Coordinates *);

void renderPresent();

void gameReport(Game*);

#endif
