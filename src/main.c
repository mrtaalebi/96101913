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
    SDL_Delay(200);
    int condition;
    int millisPerCycle = (int) (1000.0f / CYCLES_PER_SECOND);
    int max = 0;
    while (true) {
        int startCycleTime = SDL_GetTicks();
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
        } else if (condition == CONTINUE);
        int thisCycleMillis = SDL_GetTicks() - startCycleTime;
        printf("%d\n", thisCycleMillis);
        max = (thisCycleMillis > max) ? thisCycleMillis : max;
        while (!SDL_TICKS_PASSED(SDL_GetTicks(), startCycleTime + millisPerCycle));
    }
    printf("max: %d\n", max);
    return 0;
}
