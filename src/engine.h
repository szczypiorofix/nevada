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
    int scale;
    int minScale;
    int maxScale;
    int tilesOnScreenFromCenterX;
    int tilesOnScreenFromCenterY;
    SDL_Event event;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Mix_Music *music;

    int musicVolume;
    long lastTime;
	double delta;
	long timer;
	int updates;
	int frames;
	long now;
	float amountOfTicks;
	int fps_count;
	int ticks_count;
	double ns;
    short int fpsCap;
    int displayMode;
} Engine;



// ------------------ PUBLIC FUNCTIONS ------------------

Engine* engineStart(void);
void engineStop(Engine** engine);
void updateDeltaTime(Engine* engine);
void engineDelay(Engine* engine);
int loadMusic(Engine* engine, char* musicFile);

#endif
