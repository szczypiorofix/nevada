#ifndef _MAIN_H_
#define _MAIN_H_
#pragma once

#include "engine.h"


// NPC's count
#define NPC_COUNT 1

// ASM function - compare two int's
short compare(int, int);


extern Engine* engine;
Player* player;
SDL_Renderer* renderer;
Level* level;
SpriteSheet* spriteSheetAssets[2];


const unsigned int framesPlayerLeft[]  = {16, 17, 18};
const unsigned int framesPlayerRight[] = {28, 29, 30};
const unsigned int framesPlayerUp[]    = {40, 41, 42};
const unsigned int framesPlayerDown[]  = {4 , 5 , 6 };

NPC* npcs[NPC_COUNT];


#endif
