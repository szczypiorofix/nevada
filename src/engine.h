#ifndef _ENGINE_H_
#define _ENGINE_H_
#pragma once


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "defines.h"
#include "textures.h"


// ------------------ STRUCTS ------------------


typedef struct Engine {
    int started;
    int quit;
    short int scale;
    short int minScale;
    short int maxScale;
    short int tilesOnScreenFromCenterX;
    short int tilesOnScreenFromCenterY;
    SDL_Event event;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Mix_Music *music;

    unsigned short int musicVolume;
    float fps;
    float FPS;
    double previous;
    double lag;
    unsigned int ticks;
    unsigned int TICKS;
    unsigned short int fpsCap;
    unsigned int displayMode;
} Engine;



// ------------------ PUBLIC FUNCTIONS ------------------

Engine* engineStart(void);
void engineStop(Engine** engine);
void updateDeltaTime(Engine* engine);
void engineDelay(Engine* engine);
int loadMusic(Engine* engine, char* musicFile);

#endif
