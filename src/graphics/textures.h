#ifndef TEXTURES_H_
#define TEXTURES_H_

#include <SDL.h>
#include <SDL_image.h>
#include "../core/game.h"

typedef struct
{
    SDL_Texture* mTexture;
    int width;
    int height;
} Texture;


void renderTexture(Texture* t, SDL_Game* game, SDL_Rect* clip, int x, int y, int width, int height);

Texture* loadTexture(const char* fileName, SDL_Game* game);

void freeTexture(Texture* t);

void nextFrame(Texture* t);

SDL_Rect* getSpriteCR(Texture* t, int col, int row, int width, int height);

SDL_Rect* getSpriteI(Texture* t, int index, int width, int height);

#endif
