#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "view.h"

SDL_Window* win;
SDL_Surface* screen;

void blit(SDL_Surface* icon, int x, int y, int h, int w) ;

void logSDLError() {
    printf("SDL error:\n%s\n", SDL_GetError());
}

void initiateWindow() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logSDLError();
    }
    win = SDL_CreateWindow("Let's play Pacman", 100, 100, SCREEN_HEIGHT, SCREEN_WIDTH, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        logSDLError();
        SDL_Quit();
    }
    screen = SDL_GetWindowSurface(win);
    if (screen == NULL) {
        SDL_DestroyWindow(win);
        logSDLError();
        SDL_Quit();
    }
}

void paintStage(Game* game) {
    for (int i = 0; i < game->stage.n; ++i) {
        for (int j = 0; j < game->stage.m; ++j) {
            paintBackgrounds(i, j, game->stage.tiles[i][j]);
        }
    }
    renderACharacter(&game->pacman.coordinates);
    renderACharacter(&game->blinky.coordinates);
    renderACharacter(&game->pinky.coordinates);
    renderACharacter(&game->clyde.coordinates);
    renderACharacter(&game->inky.coordinates);
}

void paintCharacter(Coordinates *lastOn) {
    renderACharacter(lastOn);
}

void renderPresent() {
    SDL_UpdateWindowSurface(win);
}

void paintBackgrounds(int x, int y, char tile) {
    SDL_Surface* icon = NULL;
    switch (tile) {
        case CHEESE: icon = IMG_Load("res/icons/cheese_icon.png");
            break;
        case CHERRY: icon = IMG_Load("res/icons/cherry_icon.png");
            break;
        case PINEAPPLE: icon = IMG_Load("res/icons/pineapple_icon.png");
            break;
        case EMPTY: icon = IMG_Load("res/icons/empty_icon.png");
            break;
        case BLOCK: icon = IMG_Load("res/icons/block_icon.png");
            break;
        default: break;
    }
    blit(icon, y * TILE, x * TILE, TILE, TILE);
    SDL_FreeSurface(icon);
}

void renderACharacter(Coordinates *coordinates) {
    SDL_Surface* icon = NULL;
    if (coordinates->isDefensive) {
        icon = IMG_Load("res/icons/blue_icon.png");
    } else switch (coordinates->characterType) {
        case CHARACTER_PACMAN: icon = IMG_Load("res/icons/pacman_icon.png");
            break;
        case CHARACTER_BLINKY: icon = IMG_Load("res/icons/blinky_icon.png");
            break;
        case CHARACTER_PINKY: icon = IMG_Load("res/icons/pinky_icon.png");
            break;
        case CHARACTER_CLYDE: icon = IMG_Load("res/icons/clyde_icon.png");
            break;
        case CHARACTER_INKY: icon = IMG_Load("res/icons/inky_icon.png");
            break;
    }
    blit(icon, coordinates->currentPosition.y, coordinates->currentPosition.x, TILE, TILE);
    SDL_FreeSurface(icon);
}

void blit(SDL_Surface* icon, int x, int y, int h, int w) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.h = h;
    dst.w = w;
    SDL_BlitScaled(icon, NULL, screen, &dst);
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
