#ifndef TEXTURES_H_
#define TEXTURES_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game.h"

typedef struct Texture
{
    SDL_Texture* mTexture;
    unsigned short width;
    unsigned short height;
    unsigned short sWidth;
    unsigned short sHeight;
} Texture;


void renderTexture(Texture* t, SDL_Game* game, SDL_Rect* clip, int x, int y, unsigned short width, unsigned short height);

Texture* loadSpriteSheet(const char* fileName, SDL_Game* game, unsigned short spriteWidth, unsigned short spriteHeigth);

void freeTexture(Texture* t);

void nextFrame(Texture* t);


SDL_Rect* getSpriteI(Texture* t, int index, unsigned short width, unsigned short height);

SDL_Rect* createRectsForSprites(Level* level, const unsigned short size, Texture* t);

#endif
