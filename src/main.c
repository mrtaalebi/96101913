#include <stdbool.h>
#include <stdio.h>

#include "view.h"
#include "input.h"
#include "controler.h"

int main() {
    initiateWindow();
    Game game;
    initialize(&game);
    int millisPerCycle = (int) (1000.0f / CYCLES_PER_SECOND);
    int millisPerCycleInMenu = (int) (1000.0f / ON_MENU_CYCLES_PER_SECOND);
    NowOn nowOn = PAUSE_MENU;
    // look out: each part has its own loop and all are in games loop
    bool lost = false;
    bool frameOn = true;
    while (frameOn) {
        switch (nowOn) {
            case PLAYING_GAME:
                nowOn = PLAYING_GAME;
                startPlayingGame(&game);
                bool onGame = true;
                while (onGame) {
                    int startCycleTime = SDL_GetTicks();

                    int event = listener();
                    if (event == KEY_ESCAPE) {
                        nowOn = PAUSE_MENU;
                        onGame = false;
                        shadeBackground();
                        continue;
                    }
                    int condition = runACycle(&game, event);
                    if (condition == NEW_STAGE) {
                        gameReport(&game);
                        makeANewRoom(&game);
                        ready(&game);
                        break;
                    } else if (condition == GAME_OVER) {
                        nowOn = HALL_OF_FAME;
                        onGame = false;
                        lost = true;
                    } else if (condition == CONTINUE);

                    while (!SDL_TICKS_PASSED(SDL_GetTicks(), startCycleTime + millisPerCycle));
                }
                break;
            case PAUSE_MENU:
                nowOn = PAUSE_MENU;
                Menu* pauseMenu = startPauseMenu();
                bool onPause = true;
                while (onPause) {
                    int startCycleTime = SDL_GetTicks();

                    int event = listener();
                    int condition = runPauseMenuACycle(pauseMenu, event);
                    switch (condition) {
                        case PLAYING_GAME:
                            if (! lost) {
                                onPause = false;
                                nowOn = PLAYING_GAME;
                            }
                            break;
                        case HALL_OF_FAME:
                            onPause = false;
                            nowOn = HALL_OF_FAME;
                            break;
                        case HELP:
                            onPause = false;
                            nowOn = HELP;
                            break;
                        case CREDITS:
                            onPause = false;
                            nowOn = CREDITS;
                            break;
                        case EXIT:
                            onPause = 0;
                            frameOn = false;
                        default:
                            break;
                    }

                    while (!SDL_TICKS_PASSED(SDL_GetTicks(), startCycleTime + millisPerCycleInMenu));
                }
                break;
            case HALL_OF_FAME:
                nowOn = HALL_OF_FAME;
                startHallOfFame();
                bool onHall = true;
                while (onHall) {
                    int event = listener();
                    if (event == KEY_ESCAPE) {
                        onHall = false;
                        nowOn = PAUSE_MENU;
                    }
                }
                break;
            case HELP:
                nowOn = HELP;
                startHelp();
                bool onHelp = true;
                while (onHelp) {
                    int event = listener();
                    if (event == KEY_ESCAPE) {
                        onHelp = false;
                        nowOn = PAUSE_MENU;
                    }
                }
                break;
            case CREDITS:
                nowOn = CREDITS;
                startCredits();
                bool onCredits = true;
                while (onCredits) {
                    int event = listener();
                    if (event == KEY_ESCAPE) {
                        onCredits = false;
                        nowOn = PAUSE_MENU;
                    }
                }
                break;
        }
    }
    return 0;
}
