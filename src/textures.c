#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "textures.h"


// FORWARD DECLARATION
void renderTexture(Texture* t, SDL_Game* game, SDL_Rect* clip, int x, int y, unsigned short width, unsigned short height);
Texture* loadSpriteSheet(const char* fileName, SDL_Game* game, unsigned short spriteWidth, unsigned short spriteHeigth);
void freeTexture(Texture* t);
SDL_Rect* getSpriteI(Texture* t, int index, unsigned short width, unsigned short height);
SDL_Rect* createRectsForSprites(Level* level, int layerCount, const unsigned short size, Texture* t);
Texture* loadFromRenderedText(const char* textureText, SDL_Game* game);
void renderText(Texture* t, SDL_Game* game, int x, int y, int w, int h);




void freeTexture(Texture* t) {
    if (t->mTexture != NULL) {
        SDL_DestroyTexture(t->mTexture);
        t->mTexture = NULL;
        free(t);
    }
}


void renderTexture(Texture* t, SDL_Game* game, SDL_Rect* clip, int x, int y, unsigned short width, unsigned short height) {
    SDL_Rect renderQuad = {x, y, width, height};
    SDL_RenderCopy(game->gRenderer, t->mTexture, clip, &renderQuad);
}


Texture* loadSpriteSheet(const char* fileName, SDL_Game* game, unsigned short spriteWidth, unsigned short spriteHeigth) {
    Texture* t = malloc(sizeof(Texture));
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
    t->sWidth = spriteWidth;
    t->sHeight = spriteHeigth;
    return t;
}


SDL_Rect* getSpriteI(Texture* t, int index, unsigned short width, unsigned short height) {
    SDL_Rect* r = malloc(sizeof(SDL_Rect));
    int col = t->width / width;
    //int row = t->height / height;
    r->x = ((index - 1) % col) * width;
    r->y = ((index - 1) / col) * height;
    //printf("Cols: %i, Rows: %i, index:%i, x:%i, y:%i, w:%i, h:%i\n", col, row, index, r->x, r->y, width, height);
    r->w = width;
    r->h = height;
    return r;
}

SDL_Rect* createRectsForSprites(Level* level, int layerCount, const unsigned short size, Texture* t) {
    SDL_Rect* l = malloc(sizeof(SDL_Rect) * size);
    for (int i = 0; i < level->size; i++)
        l[i] = *getSpriteI(t, level->content[layerCount].data[i], t->sWidth, t->sHeight);
    return l;
}

void renderText(Texture* t, SDL_Game* game, int x, int y, int w, int h) {
    SDL_Rect renderQuad = {x, y, w, h};
    SDL_RenderCopy(game->gRenderer, t->mTexture, NULL, &renderQuad);
}

Texture* loadFromRenderedText(const char* textureText, SDL_Game* game) {
    assert(game != NULL && textureText != NULL);
    Texture* t = malloc(sizeof(Texture));
    if (t == NULL) return NULL;
    TTF_Font *gFont = NULL;
    gFont = TTF_OpenFont("res/camingo.ttf", 28);
    if (gFont == NULL) {
        printf("Unable to create texture from %s! SDL Error: %s\n", "res/camingo.ttf", SDL_GetError());
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
