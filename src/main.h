#ifndef _MAIN_H_
#define _MAIN_H_
#pragma once

#include "engine.h"



#define NPC_COUNT 1


short compare(int, int);

extern Engine* engine;
Player* player;
SDL_Renderer* renderer;
Level* level;
SpriteSheet* spriteSheetAssets[2];

unsigned int framesPlayerLeft[]  = {16, 17, 18};
unsigned int framesPlayerRight[] = {28, 29, 30};
unsigned int framesPlayerUp[]    = {40, 41, 42};
unsigned int framesPlayerDown[]  = {4 , 5 , 6 };

NPC* npcs[NPC_COUNT];


#endif
