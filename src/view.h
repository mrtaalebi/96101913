#ifndef VIEW_H
#define VIEW_H

#include "models.h"
#include "gui.h"

void initiateWindow();

void ready(Game* game);

void paintStage(Game*);

void drawWalls();

void paintBackgrounds(int, int, char);

void renderPresent();

void drawGhost(Ghost*, SDL_Color);

void drawPacman(Coordinates*);

void drawStatistics(Game *);

void drawText(char*, int, int, SDL_Color);

void drawSavedTexts();

void gameReport(Game*);

void shadeBackground();

void drawPauseMenu(Menu* menu);

void drawCredits();

void drawHelp();

void drawHallOfFame(char* name, int score);

#endif
