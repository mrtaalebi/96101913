#include <stdbool.h>
#include <stdio.h>

#include "view.h"
#include "input.h"
#include "controler.h"

int main() {
    initiateWindow();
    Game game;
    initialize(&game);
    paintStage(&game);
    SDL_Delay(2000);
    int condition;
    int millisPerCycle = (int) (1000.0 / CYCLES_PER_SECOND);
    while (true) {
        int startCycleTime = SDL_GetTicks();
        gameReport(&game);
        condition = runACycle(&game);
        if (condition == NEW_STAGE) {
            gameReport(&game);
            printf("\nGreat! You made it! Going for next stage!\n");
            makeANewRoom(&game);
            paintStage(&game);
            SDL_Delay(1000);
        } else if (condition == GAME_OVER) {
            printf("\nOops! You lose!\n");
            printf("Your final score is: %d\n", game.pacman.score.totalScore);
            SDL_Delay(1000);
            break;
        } else if (condition == CONTINUE){
        }
        while (!SDL_TICKS_PASSED(SDL_GetTicks(), startCycleTime + millisPerCycle));
    }
    return 0;
}
