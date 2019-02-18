#ifndef TEXTURES_H_
#define TEXTURES_H_
#pragma once


#include <SDL2/SDL.h>
#include "level.h"
#include "defines.h"



typedef struct Texture {
    SDL_Texture* mTexture;
    char* name;
    enum SpriteSheets spriteSheet;
    int width;
    int height;
    int sWidth;
    int sHeight;
} Texture;

typedef struct Animation {
    int size;
    SDL_Rect* frames;
    int speed;
    int curFrame;
    int counter;
    Texture* spriteSheet;
} Animation;




// ------------------ "PUBLIC" FUNCTIONS ------------------

Texture* loadSpriteSheet(char* fileName, enum SpriteSheets spritesheet, SDL_Renderer* renderer, unsigned int spriteWidth, unsigned int spriteHeigth);
void freeTexture(Texture* t);
void renderTexture(Texture* t, SDL_Renderer* renderer, SDL_Rect* clip, int x, int y);
SDL_Rect* createRectsForSprites(Level* level, int layerCount, const unsigned int size, Texture* t);
Animation* prepareAnimation(Texture* t, unsigned int speed, unsigned int sw, unsigned int sh, const unsigned int size, unsigned int* frames);
int nextFrame(Animation* an);
int releaseAnimation(Animation** an);

int checkCollision(SDL_Rect r1, SDL_Rect r2);

#endif
