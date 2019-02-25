#ifndef ENGINE_H_
#define ENGINE_H_
#pragma once


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "defines.h"
#include "textures.h"
#include "assetslist.h"


#define ENGINE_FPS_MAX 60

// ------------------ STRUCTS ------------------


typedef struct Engine
{
    bool started;
    bool quit;
    short int scale;
    short int minScale;
    short int maxScale;
    SDL_Event event;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Mix_Music *music;
    unsigned long int startTick;
    unsigned long int endTick;
    unsigned short int fpsCounter;
    unsigned short int musicVolume;
    float delayTime;
    float deltaTime;
    unsigned short int fps;
    AssetsList* assets;
} Engine;



// ------------------ PUBLIC FUNCTIONS ------------------

Engine* engineStart(void);
void engineStop(Engine** engine);
void updateDeltaTime(Engine* engine);
void engineDelay(Engine* engine);
int loadMusic(Engine* engine, char* musicFile);

#endif
