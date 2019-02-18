#ifndef OBJECTS_H_
#define OBJECTS_H_
#pragma once

#include "SDL2/SDL.h"

// ------------------ STRUCTS & TYPES ------------------
typedef enum {
    DIR_UP,
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT,
    DIR_UP_RIGHT,
    DIR_DOWN_RIGHT,
    DIR_DOWN_LEFT,
    DIR_UP_LEFT
} Direction;

typedef struct Camera {
    int x;
    int y;
    int offsetX;
    int offsetY;
} Camera;


typedef struct Player {
    int x;
    int y;
    int velX;
    int velY;
    int width;
    int height;
    int tileX;
    int tileY;
    int tileIndex;
    int isMoving;
    Direction direction;
    SDL_Rect col_up;
    SDL_Rect col_right;
    SDL_Rect col_down;
    SDL_Rect col_left;
} Player;


typedef struct NPC {
    int x;
    int y;
    int velX;
    int velY;
    int width;
    int height;
    int takingAction;
    int takingActionCounter;
    int maxTakingActionCounter;
    Direction direction;
    SDL_Rect col_up;
    SDL_Rect col_right;
    SDL_Rect col_down;
    SDL_Rect col_left;
} NPC;



// ------------------ PUBLIC FUNCTIONS ------------------
Player* resetPlayer(void);
NPC* setNPC(int x, int y, int width, int height, Direction direction);
void updateCamera(Camera* c, Player player);
int getTileX(Player* p, unsigned int tw);
int getTileY(Player* p, unsigned int th);

int updateNPC(NPC* npc);
void updateCollisionsNPC(NPC* npc, Camera* cam);
void updateCollisionsPlayer(Player* p, Camera* cam);


#endif
