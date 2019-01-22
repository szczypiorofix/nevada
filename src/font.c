#include <stdio.h>
#include <stdlib.h>
#include "font.h"


Texture* loadFromRenderedText(const char* textureText, SDL_Game* game);
void renderText(Texture* t, SDL_Game* game, int x, int y);





void renderText(Texture* t, SDL_Game* game, int x, int y) {
    SDL_Rect renderQuad = {x, y, 500, 80};
    SDL_RenderCopy(game->gRenderer, t->mTexture, NULL, &renderQuad);
}

Texture* loadFromRenderedText(const char* textureText, SDL_Game* game) {
    Texture* t = malloc(sizeof(Texture));
    TTF_Font *gFont = NULL;
    gFont = TTF_OpenFont("res/vinque.ttf", 28);
    if (gFont == NULL) {
        printf("Unable to create texture from %s! SDL Error: %s\n", "vinque.ttf", SDL_GetError());
    }
    SDL_Color textColor = {0xFF, 0x65, 0x00};
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText, textColor);
    if (textSurface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    } else {
        t->mTexture = SDL_CreateTextureFromSurface(game->gRenderer, textSurface);
        if (t->mTexture == NULL) {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        } else {
            t->width = textSurface->w;
            t->height = textSurface->h;
        }
        SDL_FreeSurface(textSurface);
    }
    return t;
}
