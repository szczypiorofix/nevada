#ifndef TEXTURES_H_
#define TEXTURES_H_
#pragma once


#include <SDL2/SDL.h>
#include "level.h"
#include "defines.h"



typedef struct SpriteSheet {
    SDL_Texture* mTexture;
    char* name;
    int width;
    int height;
    int tileWidth;
    int tileHeight;
} SpriteSheet;

typedef struct Animation {
    int size;
    SDL_Rect* frames;
    int speed;
    int curFrame;
    int counter;
    SpriteSheet* spriteSheet;
} Animation;




// ------------------ "PUBLIC" FUNCTIONS ------------------


SpriteSheet* loadSpriteSheet(char* fileName, SDL_Renderer* renderer, unsigned int spriteWidth, unsigned int spriteHeigth);

void freeTexture(SpriteSheet* t);

void renderTexture(SpriteSheet* t, SDL_Renderer* renderer, SDL_Rect* clip, int x, int y, int scale, double angle, SDL_Point* center, SDL_RendererFlip flip);

SDL_Rect* createRectsForSprites(Level* level, int layerCount, const unsigned int size, SpriteSheet* t);

Animation* prepareAnimation(SpriteSheet* t, unsigned int speed, unsigned int sw, unsigned int sh, const unsigned int size, unsigned int* frames);

int nextFrame(Animation* an);

int releaseAnimation(Animation** an);

int checkCollision(SDL_Rect r1, SDL_Rect r2);


#endif
