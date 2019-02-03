#ifndef TEXTURES_H_
#define TEXTURES_H_
#pragma once


#include <SDL2/SDL.h>
#include "defines.h"

typedef struct Texture {
    SDL_Texture* mTexture;
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
} Animation;


Texture* loadSpriteSheet(char* fileName, enum SpriteSheets spritesheet, SDL_Renderer* renderer, unsigned int spriteWidth, unsigned int spriteHeigth);


#endif
