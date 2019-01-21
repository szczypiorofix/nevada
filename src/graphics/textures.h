#ifndef TEXTURES_H_
#define TEXTURES_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../core/game.h"

typedef struct
{
    SDL_Texture* mTexture;
    unsigned short width;
    unsigned short height;
    unsigned short sWidth;
    unsigned short sHeight;
} Texture;


void renderTexture(Texture* t, SDL_Game* game, SDL_Rect* clip, int x, int y, int width, int height);

Texture* loadSpriteSheet(const char* fileName, SDL_Game* game, unsigned short spriteWidth, unsigned short spriteHeigth);

void freeTexture(Texture* t);

void nextFrame(Texture* t);

SDL_Rect* getSpriteXY(Texture* t, int x, int y, int width, int height);

SDL_Rect* getSpriteI(Texture* t, int index, int width, int height);

SDL_Rect* createRectsForSprites(Level* level, const short size, Texture* t);

#endif
