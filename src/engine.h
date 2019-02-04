#ifndef ENGINE_H_
#define ENGINE_H_
#pragma once


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "defines.h"
#include "textures.h"


#define ENGINE_FPS_MAX 60

// ------------------ STRUCTS ------------------


typedef struct Assets {
    short spriteSheetsCount;
    Texture* spriteSheets[];
} Assets;


typedef struct Engine
{
    bool started;
    bool quit;
    SDL_Event event;
    SDL_Window* window;
    SDL_Renderer* renderer;
    unsigned long int startTick;
    unsigned long int endTick;
    unsigned short int fpsCounter;
    float delayTime;
    float deltaTime;
    unsigned short int fps;
    Assets* assets;
} Engine;



// ------------------ PUBLIC FUNCTIONS ------------------

Engine* engineStart(void);
void engineStop(Engine* engine);
void updateDeltaTime(Engine* engine);
void engineDelay(Engine* engine);
Assets* createAssets(void);

bool addGraphicsToAssets(Texture* texture, Assets* assets);


#endif
