#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "defines.h"
#include "main.h"
#include "level.h"


/**
 * #################################################
 * ................... FUNCTIONS ...................
 * #################################################
 * */

Level* getLevel(short n);

void updateCamera(Camera* c, Player player);

SDL_Game* initGame();


int getTileX(Player* p, Camera* c, Level* l, unsigned int tw);

int getTileY(Player* p, Camera* c, Level* l, unsigned int th);

Animation* prepareAnimation(Texture* t, unsigned int speed, unsigned int sw, unsigned int sh, const unsigned int size, unsigned int* frames);

int nextFrame(Animation* an);
