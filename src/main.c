#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "view.h"
#include "input.h"
#include "controler.h"

int main() {
    SDL_Renderer* panel = initiateWindow();
    Game game;
    initialize(&game);
    int condition;
    while (true) {
    paintStage(&game, panel);
    gameReport(&game);
    condition = runACycle(&game);
    if (condition == NEW_STAGE) {
            gameReport(&game);
            printf("\nGreat! You made it! Going for next stage!\n");
            makeANewRoom(&game);
        } else if (condition == GAME_OVER) {
            printf("\nOops! You lose!\n");
            printf("Your final score is: %d\n", game.pacman.score.totalScore);
            break;
        } else if (condition == CONTINUE){
            continue;
        }
    }
    return 0;
}
