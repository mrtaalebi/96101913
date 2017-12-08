#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "view.h"
#include "models.h"

SDL_Renderer* ren;

void logSDLError() {
    printf("SDL error:\n%s\n", SDL_GetError());
}

void initiateWindow() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logSDLError();
    }
    SDL_Window *win = SDL_CreateWindow("Let's play Pacman", 100, 100, SCREEN_HEIGHT, SCREEN_WIDTH, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        logSDLError();
        SDL_Quit();
    }
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == NULL) {
        SDL_DestroyWindow(win);
        logSDLError();
        SDL_Quit();
    }
}

void paintStage(Game* game) {
    SDL_RenderClear(ren);
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

    SDL_RenderPresent(ren);
}

void paintCharacter(Coordinates *lastOn, Point *movedTo, Stage *stage) {
    //todo: fuck this shit
    cleanACorpse(lastOn);
    lastOn->currentPosition.x = movedTo->x;
    lastOn->currentPosition.y = movedTo->y;
    renderACharacter(lastOn);
    SDL_RenderPresent(ren);
}

void paintBackgrounds(double x, double y, char tile) {
    SDL_Texture* icon = NULL;
    switch (tile) {
        case CHEESE: icon = loadTexture("res/icons/cheese_icon.png");
            break;
        case CHERRY: icon = loadTexture("res/icons/cherry_icon.png");
            break;
        case PINEAPPLE: icon = loadTexture("res/icons/pineapple_icon.png");
            break;
        case EMPTY: icon = loadTexture("res/icons/empty_icon.png");
            break;
        case BLOCK: icon = loadTexture("res/icons/block_icon.png");
            break;
        default: break;
    }
    renderTexture(icon, (int) y * TILE, (int) x * TILE, TILE, TILE);
    SDL_DestroyTexture(icon);
}

void cleanACorpse(Coordinates *coordinates) {
    SDL_DestroyTexture(coordinates->texture);
    SDL_RenderPresent(ren);
}

void renderACharacter(Coordinates *coordinates) {
    SDL_Texture* icon = NULL;
    switch (coordinates->characterType) {
        case CHARACTER_PACMAN: icon = loadTexture("res/icons/pacman_icon.png");
            break;
        case CHARACTER_BLINKY: icon = loadTexture("res/icons/blinky_icon.png");
            break;
        case CHARACTER_PINKY: icon = loadTexture("res/icons/pinky_icon.png");
            break;
        case CHARACTER_CLYDE: icon = loadTexture("res/icons/clyde_icon.png");
            break;
        case CHARACTER_INKY: icon = loadTexture("res/icons/inky_icon.png");
            break;
    }
    renderTexture(icon, coordinates->currentPosition.y, coordinates->currentPosition.x, TILE, TILE);
    coordinates->texture = icon;
}

void renderTexture(SDL_Texture *tex, int x, int y, int h, int w){
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.h = h;
    dst.w = w;
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

SDL_Texture* loadTexture(const char* file) {
    SDL_Texture *texture = IMG_LoadTexture(ren, file);
    if (texture == NULL) {
        logSDLError();
    }
    return texture;
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
