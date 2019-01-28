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


int getTileX(Player* p, Camera* c, Level* l, uint16 tw);

int getTileY(Player* p, Camera* c, Level* l, uint16 th);

Animation* prepareAnimation(Texture* t, uint16 speed, uint16 sw, uint16 sh, const uint16 size, uint16* frames);

int nextFrame(Animation* an);
