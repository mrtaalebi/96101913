#include <stdlib.h>
#include <stdio.h>
#include <SDL2_gfxPrimitives.h>
#include "gui.h"
#include "models.h"
#include "view.h"

const int ON_MENU_CYCLES_PER_SECOND = 10;

const int MENU_WIDTH = 16 * TILE, MENU_HEIGHT = 24 * TILE, BUTTON_WIDTH = 12 * TILE, BUTTON_HEIGHT = 2 * TILE;

const SDL_Color COLOR_MENU_BACKGROUND = {60, 63, 65, 255},
        COLOR_BUTTON = {135, 147, 154, 255},
        COLOR_BUTTON_HOVERED = {55, 76, 91, 255},
        COLOR_TEXT_BUTTON = {149, 209, 243, 255},
        COLOR_TEXT_CREDITS = {217, 165, 72, 255};

Menu* startPauseMenu() {
    shadeBackground();
    Menu* pauseMenuOptions = malloc(sizeof(Menu));
    pauseMenuOptions->hoveredOptionIndex = 0;
    pauseMenuOptions->numberOfOptions = 5;
    pauseMenuOptions->options->text = "TO GAME";
    (pauseMenuOptions->options + 1)->text = "HALL OF FAME";
    (pauseMenuOptions->options + 2)->text = "HELP";
    (pauseMenuOptions->options + 3)->text = "CREDITS";
    (pauseMenuOptions->options + 4)->text = "EXIT";
}

int runPauseMenuACycle(Menu* menu, int event) {
    drawPauseMenu(menu);
    switch (event) {
        case DIR_UP:
            if (menu->hoveredOptionIndex > 0)
                menu->hoveredOptionIndex--;
            break;
        case DIR_DOWN:
            if (menu->hoveredOptionIndex < menu->numberOfOptions - 1)
                menu->hoveredOptionIndex++;
            break;
        case KEY_RETURN:
            switch (menu->hoveredOptionIndex) {
                case 0:
                    return PLAYING_GAME;
                case 1:
                    return HALL_OF_FAME;
                case 2:
                    return HELP;
                case 3:
                    return CREDITS;
                case 4:
                    return EXIT;
                default:
                    return DIR_NONE;
            }
        default:
            return DIR_NONE;
    }


}

void startPlayingGame(Game* game) {
    ready(game);
}

void startHallOfFame() {
    drawHallOfFame();
}

void startCredits() {
    drawCredits();
}

void startHelp() {
    drawHelp();
}
