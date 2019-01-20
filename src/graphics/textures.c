#include <stdio.h>
#include <stdlib.h>
#include "textures.h"



void renderTexture(Texture* t, SDL_Game* game, SDL_Rect* clip, int x, int y, int width, int height);
Texture* loadTexture(const char* fileName, SDL_Game* game);
void freeTexture(Texture* t);

SDL_Rect* getSpriteCR(Texture* t, int x, int y, int width, int height);
SDL_Rect* getSpriteI(Texture* t, int index, int width, int height);




void freeTexture(Texture* t) {
    if (t->mTexture != NULL) {
        SDL_DestroyTexture(t->mTexture);
        t->mTexture = NULL;
        free(t);
    }
}


void renderTexture(Texture* t, SDL_Game* game, SDL_Rect* clip, int x, int y, int width, int height) {
    SDL_Rect renderQuad = {x, y, width, height};
    SDL_RenderCopy(game->gRenderer, t->mTexture, clip, &renderQuad );
}

Texture* loadTexture(const char* fileName, SDL_Game* game) {
    Texture* t = (Texture *)malloc(sizeof(Texture));
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(fileName);
    if (loadedSurface == NULL) {
        printf( "Unable to load image %s! SDL_image Error: %s\n", fileName, IMG_GetError());
        t = NULL;
    } else {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
        newTexture = SDL_CreateTextureFromSurface(game->gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", fileName, SDL_GetError());
        } else {
            t->width = loadedSurface->w;
            t->height = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    t->mTexture = newTexture;
    return t;
}


void nextFrame(Texture* t) {

}

SDL_Rect* getSpriteXY(Texture* t, int x, int y, int width, int height) {
    SDL_Rect* r = malloc(sizeof(SDL_Rect));
    r->x = x * width;
    r->y = y * height;
    r->w = width;
    r->h = height;
    return r;
}

SDL_Rect* getSpriteI(Texture* t, int index, int width, int height) {
    SDL_Rect* r = malloc(sizeof(SDL_Rect));
    int col = t->width / width;
    //int row = t->height / height;
    r->x = (index % col) * width;
    r->y = (index / col) * height;
    //printf("Cols: %i, Rows: %i, index:%i, x:%i, y:%i, w:%i, h:%i\n", col, row, index, r->x, r->y, width, height);
    r->w = width;
    r->h = height;
    return r;
}
