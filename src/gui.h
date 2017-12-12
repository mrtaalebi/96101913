#include <stdbool.h>
#include <SDL_surface.h>
#include <SDL_render.h>

struct {
    SDL_Surface* surface;
}typedef Button;

struct {
    char* string;
    SDL_Texture* texture;
    SDL_Color* textColor;
    //if ( !gTextTexture.loadFromRenderedText( "The quick brown fox jumps over the lazy dog", textColor ) );
} typedef Label;

struct {
    char* file;
    SDL_Surface* image;
} typedef Image;



void openIndexMenu();

void closeIndexMenu();

void startGameSheet();

void endGameSheet();

void startHighScoreSheet();

void endHighScoreSheet();