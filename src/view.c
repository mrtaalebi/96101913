#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>

#include "view.h"

const int SCREEN_WIDTH = 480, SCREEN_HEIGHT = 720;

void logSDLError() {
    printf("SDL error:\n%s\n", SDL_GetError());
}

SDL_Renderer* initiateWindow() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logSDLError();
    }
    SDL_Window *win = SDL_CreateWindow("Let's play Pacman", 100, 100, SCREEN_HEIGHT, SCREEN_WIDTH, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        logSDLError();
        SDL_Quit();
    }
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == NULL) {
        SDL_DestroyWindow(win);
        logSDLError();
        SDL_Quit();
    }
    return ren;
}

void paintStage(Game* game, SDL_Renderer* ren) {
    SDL_RenderClear(ren);
    const int TILE = 40;
    SDL_Texture *icon = NULL;
    for (int i = 0; i < game->stage.n; ++i) {
        for (int j = 0; j < game->stage.m; ++j) {
            char c = game->stage.tiles[i][j];
            switch (c) {
                case CHEESE: icon = loadTexture("res/icons/cheese_icon.png", ren);
                    break;
                case CHERRY: icon = loadTexture("res/icons/cherry_icon.png", ren);
                    break;
                case PINEAPPLE: icon = loadTexture("res/icons/pineapple_icon.png", ren);
                    break;
                case EMPTY: icon = loadTexture("res/icons/empty_icon.png", ren);
                    break;
                case BLOCK: icon = loadTexture("res/icons/block_icon.png", ren);
                    break;
                default: break;
            }
            renderTexture(icon, ren, i * TILE, j * TILE, TILE, TILE);
        }
    }
    renderACharacter("res/icons/pacman_icon.png", icon, ren, &game->pacman.coordinates, TILE);
    renderACharacter("res/icons/blinky_icon.png", icon, ren, &game->blinky.coordinates, TILE);
    renderACharacter("res/icons/pinky_icon.png", icon, ren, &game->pinky.coordinates, TILE);
    renderACharacter("res/icons/clyde_icon.png", icon, ren, &game->clyde.coordinates, TILE);
    renderACharacter("res/icons/inky_icon.png", icon, ren, &game->inky.coordinates, TILE);

    SDL_RenderPresent(ren);
}

void renderACharacter(char *file, SDL_Texture *icon, SDL_Renderer *ren, Coordinates *coordinates, int TILE) {
    icon = loadTexture(file, ren);
    renderTexture(icon, ren, coordinates->current.x * TILE, coordinates->current.y * TILE, TILE, TILE);
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int h, int w){
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.h = h;
    dst.w = w;
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

SDL_Texture* loadTexture(const char* file, SDL_Renderer *ren) {
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
    out[game->pacman.coordinates.current.x][game->pacman.coordinates.current.y] = 'C';
    out[game->blinky.coordinates.current.x][game->blinky.coordinates.current.y] = 'B';
    out[game->pinky.coordinates.current.x][game->pinky.coordinates.current.y] = 'P';
    out[game->clyde.coordinates.current.x][game->clyde.coordinates.current.y] = 'L';
    out[game->inky.coordinates.current.x][game->inky.coordinates.current.y] = 'I';
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
                printf("%s", s);
            else printf("%c", out[i][j]);
        }
        printf("\n");
    }
    printf("\n2, 4, 6, 8\n");
}
