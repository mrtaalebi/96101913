#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>

#include "view.h"
#include "utils.h"
#include "input.h"

SDL_Window* win;
SDL_Renderer* renderer;

SDL_Surface* background;
SDL_Texture* back;
SDL_Rect* rect;

SDL_Surface* littleImage;
SDL_Texture* littleTexture;
SDL_Rect* littleRect;

Name* scoreAnimated;

void logSDLError() {
    printf("SDL error:\n%s\n", SDL_GetError());
}

void initiateWindow() {

    // todo: make the window responsive :)))

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    int Width = DM.w;
    int Height = DM.h;



    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logSDLError();
    }
    win = SDL_CreateWindow("Let's play Pacman", 100, 300, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        printf("aoeu");
        logSDLError();
        SDL_Quit();
    }
    renderer = SDL_CreateRenderer(win, 0, SDL_RENDERER_ACCELERATED);
    background = IMG_Load("res/index.jpeg");
    back = SDL_CreateTextureFromSurface(renderer, background);
    rect = malloc(sizeof(SDL_Rect));
    rect->x = 0; rect->y = TILE * 3; rect->w = SCREEN_WIDTH; rect->h = SCREEN_HEIGHT - 5 * TILE;
    SDL_FreeSurface(background);
    littleImage = IMG_Load("res/pacman_index_image.jpg");
    littleTexture = SDL_CreateTextureFromSurface(renderer, littleImage);
    littleRect = malloc(sizeof(SDL_Rect));
    littleRect->x = (SCREEN_WIDTH - MENU_WIDTH) / 2 + TILE * 2;
    littleRect->y = SCREEN_HEIGHT - (SCREEN_HEIGHT - MENU_HEIGHT) / 2 - TILE * 6;
    littleRect->w = MENU_WIDTH - TILE * 4;
    littleRect->h = TILE * 5;
    SDL_FreeSurface(littleImage);


    scoreAnimated = malloc(sizeof(Name));
    scoreAnimated->string = " ";
    scoreAnimated->next = NULL;
}

void ready(Game* game) {
    paintStage(game);
    stringRGBA(renderer, (Sint16) ((Sint16) 14 * TILE - TILE / 2 * 3 + 3), (Sint16) (21 * TILE - TILE / 2), "ready!", COLOR_READY.r, COLOR_READY.g, COLOR_READY.b, COLOR_READY.a);
    renderPresent();
    SDL_Delay((Uint32) (NEW_STAGE_DELAY * 500));
}

void paintStage(Game* game) {
    drawStatistics(game);
    drawWalls();
    for (int i = 0; i < game->stage.n; ++i)
        for (int j = 0; j < game->stage.m; ++j)
            paintBackgrounds(i, j, game->stage.tiles[i][j]);
    drawPacman(&game->pacman.coordinates);
    drawGhost(&game->blinky, COLOR_BLINKY);
    drawGhost(&game->pinky, COLOR_PINKY);
    drawGhost(&game->clyde, COLOR_CLYDE);
    drawGhost(&game->inky, COLOR_INKY);
    drawSavedTexts();
    renderPresent();
}

void drawSavedTexts() {
    Name* on = scoreAnimated;
    if (on->next != NULL) {
        stringRGBA(renderer, (Sint16) (on->next->x - TILE * 2 + rand() % (TILE * 4)),
                   (Sint16) (on->next->y - TILE * 2 + rand() % (TILE * 4)),
                   on->next->string, on->next->color.r, on->next->color.g, on->next->color.b, on->next->color.a);
        on->next->cyclesRemaining--;
        if (on->next->cyclesRemaining == 0)
            on->next = on->next->next;
    }/*
    while (true) {
        stringRGBA(renderer, (Sint16) (scoreAnimated->x - TILE * 2 + rand() % (TILE * 4)),
                   (Sint16) (scoreAnimated->y - TILE * 2 + rand() % (TILE * 4)),
                   scoreAnimated->string, scoreAnimated->color.r, scoreAnimated->color.g, scoreAnimated->color.b, scoreAnimated->color.a);
        Name* on = scoreAnimated;
        scoreAnimated = scoreAnimated->next;
        if (scoreAnimated == NULL) break;
    }*/
}

void drawText(char* text, int x, int y, SDL_Color color) {
    Name* new = malloc(sizeof(Name));
    new->string = text;
    new->color = color;
    new->x = x;
    new->y = y;
    new->cyclesRemaining = SCORE_ON_CYCLES;
    new->next = NULL;
    Name* on = scoreAnimated;
    while (on->next != NULL) {
        on = on->next;
    }
    on->next = new;
}

void renderPresent() {
    SDL_RenderPresent(renderer);
}

void drawStatistics(Game *game) {

    // todo: font is too small and should be get bigger a little

    boxRGBA(renderer, 0, 0, (Sint16) SCREEN_WIDTH, (Sint16) (TILE * 3), 0, 0, 0, 255);
    boxRGBA(renderer, 0, (Sint16) (SCREEN_HEIGHT - TILE * 3), (Sint16) SCREEN_WIDTH, (Sint16) SCREEN_HEIGHT, 0, 0, 0, 255);

    char* string = malloc(20);
    sprintf(string, "%s % 4.2f", "time:", (float) game->stage.cycles / CYCLES_PER_SECOND);
    stringRGBA(renderer, (Sint16) TILE, (Sint16) (TILE * 2), string, COLOR_STATISTICS.r, COLOR_STATISTICS.g, COLOR_STATISTICS.b, COLOR_STATISTICS.a);

    sprintf(string, "%s % 5d", "score:", game->pacman.score.totalScore);
    stringRGBA(renderer, (Sint16) (SCREEN_WIDTH - TILE * 7), (Sint16) (TILE * 2), string, COLOR_STATISTICS.r, COLOR_STATISTICS.g, COLOR_STATISTICS.b, COLOR_STATISTICS.a);

    int y = SCREEN_HEIGHT - TILE;
    for (int i = 0; i < game->pacman.hearts - 1; ++i) {
        int x = TILE + TILE * i * 2;
        filledPieRGBA(renderer, (Sint16) x, (Sint16) y, (Sint16) (TILE * CHARACTER_TO_TILE * .9),
                      (Sint16) 20, (Sint16) 320, COLOR_PACMAN.r, COLOR_PACMAN.g, COLOR_PACMAN.b, COLOR_PACMAN.a);
    }

    for (int i = 0; i < game->stage.roomNumber - 1; ++i) {
        int x = SCREEN_WIDTH - TILE - TILE * i * 2;
        filledCircleRGBA(renderer, (Sint16) x, (Sint16) y, (Sint16) (TILE * CHARACTER_TO_TILE * .9),
                         COLOR_CHERRY.r, COLOR_CHERRY.g,COLOR_CHERRY.b, COLOR_CHERRY.a);
    }
}

void drawWalls() {
    SDL_RenderCopy(renderer, back, NULL, rect);
}

void paintBackgrounds(int x, int y, char tile) {
    switch (tile) {
        case CHEESE: filledEllipseRGBA(renderer, (Sint16) (y * TILE + TILE / 2), (Sint16) (x * TILE + TILE / 2), (Sint16) (TILE / 5),
                                       (Sint16) (TILE / 5), COLOR_CHEESE.r, COLOR_CHEESE.g,COLOR_CHEESE.b, COLOR_CHEESE.a);
            break;
        case CHERRY: filledEllipseRGBA(renderer, (Sint16) (y * TILE + TILE / 2), (Sint16) (x * TILE + TILE / 2), (Sint16) (TILE / 2.5),
                                       (Sint16) (TILE / 2.5), COLOR_CHERRY.r,COLOR_CHERRY.g ,COLOR_CHERRY.b ,COLOR_CHERRY.a);
            break;
        case PINEAPPLE: filledEllipseRGBA(renderer, (Sint16) (y * TILE + TILE / 2), (Sint16) (x * TILE + TILE / 2), (Sint16) (TILE / 2.5),
                                          (Sint16) (TILE / 2.5), COLOR_PINEAPPLE.r, COLOR_PINEAPPLE.g, COLOR_PINEAPPLE.b, COLOR_PINEAPPLE.a);
            break;
        case EMPTY: /// nothing more; we paint it copying an image :)
            break;
        case BLOCK: /// nothing more; we paint it copying an image :)
            break;
        default: break;
    }
}

void drawPacman(Coordinates* coordinates) {
    int centerX = coordinates->currentPosition.y + TILE / 2;
    int centerY = coordinates->currentPosition.x + TILE / 2;
    int startAngle = 1, endAngle = 0;
    switch (coordinates->direction) {
        case DIR_UP:
            startAngle = (int) (270 + (double) (coordinates->currentPosition.x % TILE + TILE / 2) / TILE * 30);
            endAngle = (int) (270 - (double) (coordinates->currentPosition.x % TILE + TILE / 2) / TILE * 30);
            break;
        case DIR_RIGHT:
            startAngle = (int) (0 + (double) (coordinates->currentPosition.y % TILE + TILE / 2) / TILE * 30);
            endAngle = (int) (0 - (double) (coordinates->currentPosition.y % TILE + TILE / 2) / TILE * 30);
            break;
        case DIR_DOWN:
            startAngle = (int) (90 + (double) (coordinates->currentPosition.x % TILE + TILE / 2) / TILE * 30);
            endAngle = (int) (90 - (double) (coordinates->currentPosition.x % TILE + TILE / 2) / TILE * 30);
            break;
        case DIR_LEFT:
            startAngle = (int) (180 + (double) (coordinates->currentPosition.y % TILE + TILE / 2) / TILE * 30);
            endAngle = (int) (180 - (double) (coordinates->currentPosition.y % TILE + TILE / 2) / TILE * 30);
            break;
        case DIR_NONE:
            break;
    }
    filledPieRGBA(renderer, (Sint16) centerX, (Sint16) centerY, (Sint16) (TILE * CHARACTER_TO_TILE),
                  (Sint16) startAngle, (Sint16) endAngle, COLOR_PACMAN.r, COLOR_PACMAN.g, COLOR_PACMAN.b, COLOR_PACMAN.a);
}

void drawGhost(Ghost* ghost, SDL_Color color) {
    int centerX = ghost->coordinates.currentPosition.y + TILE / 2;
    int centerY = ghost->coordinates.currentPosition.x + TILE / 2;
    if (ghost->coordinates.isDefensive) color = COLOR_BLUE_GHOST;
    if (ghost->defensiveCyclesLeft >= 0) {
        filledPieRGBA(renderer, (Sint16) centerX, (Sint16) centerY,
                      (Sint16) (TILE * CHARACTER_TO_TILE),
                      180, 0, color.r, color.g, color.b, color.a);
        boxRGBA(renderer, (Sint16) (centerX - TILE * CHARACTER_TO_TILE), (Sint16) (centerY),
                (Sint16) (centerX + TILE * CHARACTER_TO_TILE), (Sint16) (centerY + TILE * CHARACTER_TO_TILE / 2),
                color.r, color.g, color.b, color.a);
        for (int i = 0; i < 3; ++i) {
            filledEllipseRGBA(renderer,
                              (Sint16) (centerX - TILE * CHARACTER_TO_TILE + TILE * CHARACTER_TO_TILE / 3 +
                                        TILE * CHARACTER_TO_TILE / 3 * 2 * i),
                              (Sint16) (centerY + TILE * CHARACTER_TO_TILE / 2),
                              (Sint16) (TILE * CHARACTER_TO_TILE / 3), (Sint16) (TILE * CHARACTER_TO_TILE / 2),
                              color.r, color.g, color.b, color.a);
        }
    }
    switch (ghost->coordinates.direction) {
        case DIR_UP:
            filledCircleRGBA(renderer, (Sint16) (centerX - TILE * CHARACTER_TO_TILE / 3), (Sint16) (centerY - TILE * CHARACTER_TO_TILE / 2), (Sint16) (TILE * CHARACTER_TO_TILE * .3),
                             COLOR_EYE.r, COLOR_EYE.g, COLOR_EYE.b, COLOR_EYE.a);
            filledCircleRGBA(renderer, (Sint16) (centerX + TILE * CHARACTER_TO_TILE / 3), (Sint16) (centerY - TILE * CHARACTER_TO_TILE / 2), (Sint16) (TILE * CHARACTER_TO_TILE * .3),
                             COLOR_EYE.r, COLOR_EYE.g, COLOR_EYE.b, COLOR_EYE.a);
            filledCircleRGBA(renderer, (Sint16) (centerX - TILE * CHARACTER_TO_TILE / 3), (Sint16) (centerY - TILE * CHARACTER_TO_TILE / 2 - TILE * CHARACTER_TO_TILE / 6), (Sint16) (TILE * CHARACTER_TO_TILE * .15),
                             COLOR_PEOPLE.r, COLOR_PEOPLE.g, COLOR_PEOPLE.b, COLOR_PEOPLE.a);
            filledCircleRGBA(renderer, (Sint16) (centerX + TILE * CHARACTER_TO_TILE / 3), (Sint16) (centerY - TILE * CHARACTER_TO_TILE / 2 - TILE * CHARACTER_TO_TILE / 6), (Sint16) (TILE * CHARACTER_TO_TILE * .15),
                             COLOR_PEOPLE.r, COLOR_PEOPLE.g, COLOR_PEOPLE.b, COLOR_PEOPLE.a);
            break;
        case DIR_RIGHT:
            filledCircleRGBA(renderer, (Sint16) (centerX + TILE * CHARACTER_TO_TILE - TILE * CHARACTER_TO_TILE / 6), (Sint16) (centerY - TILE * CHARACTER_TO_TILE / 3), (Sint16) (TILE * CHARACTER_TO_TILE * .3),
                             COLOR_EYE.r, COLOR_EYE.g, COLOR_EYE.b, COLOR_EYE.a);
            filledCircleRGBA(renderer, (Sint16) (centerX + TILE * CHARACTER_TO_TILE), (Sint16) (centerY - TILE * CHARACTER_TO_TILE / 3), (Sint16) (TILE * CHARACTER_TO_TILE * .15),
                             COLOR_PEOPLE.r, COLOR_PEOPLE.g, COLOR_PEOPLE.b, COLOR_PEOPLE.a);
            break;
        case DIR_DOWN:
            filledCircleRGBA(renderer, (Sint16) (centerX - TILE * CHARACTER_TO_TILE / 3), (Sint16) (centerY - TILE * CHARACTER_TO_TILE / 4), (Sint16) (TILE * CHARACTER_TO_TILE * .3),
                             COLOR_EYE.r, COLOR_EYE.g, COLOR_EYE.b, COLOR_EYE.a);
            filledCircleRGBA(renderer, (Sint16) (centerX + TILE * CHARACTER_TO_TILE / 3), (Sint16) (centerY - TILE * CHARACTER_TO_TILE / 4), (Sint16) (TILE * CHARACTER_TO_TILE * .3),
                             COLOR_EYE.r, COLOR_EYE.g, COLOR_EYE.b, COLOR_EYE.a);
            filledCircleRGBA(renderer, (Sint16) (centerX - TILE * CHARACTER_TO_TILE / 3), (Sint16) (centerY - TILE * CHARACTER_TO_TILE / 4 + TILE * CHARACTER_TO_TILE / 6), (Sint16) (TILE * CHARACTER_TO_TILE * .15),
                             COLOR_PEOPLE.r, COLOR_PEOPLE.g, COLOR_PEOPLE.b, COLOR_PEOPLE.a);
            filledCircleRGBA(renderer, (Sint16) (centerX + TILE * CHARACTER_TO_TILE / 3), (Sint16) (centerY - TILE * CHARACTER_TO_TILE / 4 + TILE * CHARACTER_TO_TILE / 6), (Sint16) (TILE * CHARACTER_TO_TILE * .15),
                             COLOR_PEOPLE.r, COLOR_PEOPLE.g, COLOR_PEOPLE.b, COLOR_PEOPLE.a);
            break;
        case DIR_LEFT:
            filledCircleRGBA(renderer, (Sint16) (centerX - TILE * CHARACTER_TO_TILE + TILE * CHARACTER_TO_TILE / 6), (Sint16) (centerY - TILE * CHARACTER_TO_TILE/ 3), (Sint16) (TILE * CHARACTER_TO_TILE * .3),
                             COLOR_EYE.r, COLOR_EYE.g, COLOR_EYE.b, COLOR_EYE.a);
            filledCircleRGBA(renderer, (Sint16) (centerX - TILE * CHARACTER_TO_TILE), (Sint16) (centerY - TILE * CHARACTER_TO_TILE / 3), (Sint16) (TILE * CHARACTER_TO_TILE * .15),
                             COLOR_PEOPLE.r, COLOR_PEOPLE.g, COLOR_PEOPLE.b, COLOR_PEOPLE.a);
            break;
        case DIR_NONE:break;
    }
}

void gameReport(Game *game) {
    char out[game->stage.n][game->stage.m];
    for (int i = 0; i < game->stage.n; ++i)
        for (int j = 0; j < game->stage.m; ++j)
            out[i][j] = game->stage.tiles[i][j];
    out[game->pacman.coordinates.currentPosition.x / TILE][game->pacman.coordinates.currentPosition.y / TILE] = 'C';
    out[game->blinky.coordinates.currentPosition.x / TILE][game->blinky.coordinates.currentPosition.y / TILE] = 'B';
    out[game->pinky.coordinates.currentPosition.x / TILE][game->pinky.coordinates.currentPosition.y / TILE] = 'P';
    out[game->clyde.coordinates.currentPosition.x / TILE][game->clyde.coordinates.currentPosition.y / TILE] = 'L';
    out[game->inky.coordinates.currentPosition.x / TILE][game->inky.coordinates.currentPosition.y / TILE] = 'I';
    for (int i = 0; i < game->stage.n; ++i) {
        for (int j = 0; j < game->stage.m; ++j) {
            char *s = NULL;
            switch (out[i][j]) {
                case '_': s = "░"; break;
                case '#': s = "█"; break;
                case '*': s = "▪"; break;
                case '^': s = "♪"; break;
                case 'O': s = "●"; break;
                default: s = "aoeu"; break;
            }
            if (s != "aoeu")
                printf("%s ", s);
            else printf("%c ", out[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

void shadeBackground() {
    boxRGBA(renderer, 0, 0, (Sint16) SCREEN_WIDTH, (Sint16) SCREEN_HEIGHT, 0, 0, 0, 127);
}

void drawMenu() {
    roundedBoxRGBA(renderer, (Sint16) ((SCREEN_WIDTH - MENU_WIDTH) / 2), (Sint16) ((SCREEN_HEIGHT - MENU_HEIGHT) / 2),
                   (Sint16) (SCREEN_WIDTH  - (SCREEN_WIDTH - MENU_WIDTH) / 2), (Sint16) (SCREEN_HEIGHT - (SCREEN_HEIGHT - MENU_HEIGHT) / 2),
                   (Sint16) TILE, COLOR_MENU_BACKGROUND.r, COLOR_MENU_BACKGROUND.g, COLOR_MENU_BACKGROUND.b, COLOR_MENU_BACKGROUND.a);
    SDL_RenderCopy(renderer, littleTexture, NULL, littleRect);
}

void drawPauseMenu(Menu* menu) {
    drawMenu();
    SDL_Color color;
    for (int i = 0; i < menu->numberOfOptions; ++i) {
        if (i == menu->hoveredOptionIndex) color = COLOR_BUTTON_HOVERED;
        else color = COLOR_BUTTON;
        roundedBoxRGBA(renderer, (Sint16) ((SCREEN_WIDTH - BUTTON_WIDTH) / 2), (Sint16) ((SCREEN_HEIGHT - MENU_HEIGHT) / 2 + BUTTON_HEIGHT * 1.5 * (i + 1)),
                       (Sint16) (SCREEN_WIDTH - (SCREEN_WIDTH - BUTTON_WIDTH) / 2), (Sint16) ((SCREEN_HEIGHT - MENU_HEIGHT) / 2 + BUTTON_HEIGHT * 1.5 * (i + 1) + BUTTON_HEIGHT),
                       (Sint16) TILE, color.r, color.g, color.b, color.a);
        color = COLOR_TEXT_BUTTON;
        stringRGBA(renderer, (Sint16) ((SCREEN_WIDTH - BUTTON_WIDTH) / 2 + TILE), (Sint16) ((SCREEN_HEIGHT - MENU_HEIGHT) / 2 + BUTTON_HEIGHT * 1.5 * (i + 1) + TILE),
                   (menu->options + i)->text ,color.r, color.g, color.b, color.a);
    }
    renderPresent();
}

void drawCredits() {
    drawMenu();
    SDL_Color color = COLOR_TEXT_CREDITS;
    stringRGBA(renderer, (Sint16) ((SCREEN_WIDTH - BUTTON_WIDTH) / 2 + TILE), (Sint16) ((SCREEN_HEIGHT - MENU_HEIGHT) / 2 + TILE),
               "credit goes here!" ,color.r, color.g, color.b, color.a);
    renderPresent();
}

void drawHelp() {
    drawMenu();
    SDL_Color color = COLOR_TEXT_CREDITS;
    stringRGBA(renderer, (Sint16) ((SCREEN_WIDTH - BUTTON_WIDTH) / 2 + TILE), (Sint16) ((SCREEN_HEIGHT - MENU_HEIGHT) / 2 + TILE),
               "help goes here!" ,color.r, color.g, color.b, color.a);
    renderPresent();
}

void drawHallOfFame(char* name, int score) {
    drawMenu();
    SDL_Color color;
    char* input = malloc(sizeof(char) * 50);
    sprintf(input, "%s %10d", name, score);
    stringRGBA(renderer, (Sint16) ((SCREEN_WIDTH - BUTTON_WIDTH) / 2), (Sint16) ((SCREEN_HEIGHT - MENU_HEIGHT) / 2 + BUTTON_HEIGHT * .25 + TILE / 2),
               input ,color.r, color.g, color.b, color.a);
    Fame* fames = readFames();
    for (int i = 0; i < 10; ++i) {
        color = COLOR_TEXT_BUTTON;
        char * string = malloc(100);
        sprintf(string, "% .2d- %s % 16d", i + 1, fames[i].name, fames[i].score.totalScore);
        stringRGBA(renderer, (Sint16) ((SCREEN_WIDTH - BUTTON_WIDTH) / 2), (Sint16) ((SCREEN_HEIGHT - MENU_HEIGHT) / 2 + BUTTON_HEIGHT * .75 * (i + 1) + TILE),
                  string ,color.r, color.g, color.b, color.a);
    }
    renderPresent();
}
