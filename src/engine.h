#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "defines.h"
#include "textures.h"


#define ENGINE_FPS_MAX 60

// ------------------ STRUCTS ------------------

struct Engine
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
};



// ------------------ PUBLIC FUNCTIONS ------------------

struct Engine* engineStart(void);
void engineStop(struct Engine* engine);
void updateDeltaTime(struct Engine* engine);
void engineDelay(struct Engine* engine);

