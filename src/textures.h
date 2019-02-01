#pragma once

#include <SDL2/SDL.h>



typedef struct Texture {
    SDL_Texture* mTexture;
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

