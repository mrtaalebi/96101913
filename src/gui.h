#ifndef GUI_H
#define GUI_H

#include "models.h"

const int ON_MENU_CYCLES_PER_SECOND;

const int MENU_WIDTH, MENU_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT;

const SDL_Color COLOR_MENU_BACKGROUND, COLOR_BUTTON, COLOR_BUTTON_HOVERED, COLOR_TEXT_BUTTON, COLOR_TEXT_CREDITS;

struct {
    char* text;
} typedef Option;

struct {
    Option options[10];
    int numberOfOptions;
    int hoveredOptionIndex;
} typedef Menu;

struct {
    char* name;
    Score score;
} typedef Fame;

Menu* startPauseMenu();

int runPauseMenuACycle(Menu*, int);

void startPlayingGame(Game* game);

void startHallOfFame();

void startCredits();

void startHelp();

#endif
