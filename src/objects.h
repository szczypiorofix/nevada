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
    Vector2 vec;
} Camera;


typedef struct Player {
    char* name;
    Vector2 vec;
    float velX;
    float velY;
    short int width;
    short int height;
    int angle;
    int angleVel;
    int speed;
    int tileX;
    int tileY;
    int tileIndex;
    short int isMoving;
    Direction direction;
    SDL_Rect col_up;
    SDL_Rect col_right;
    SDL_Rect col_down;
    SDL_Rect col_left;
} Player;


typedef struct NPC {
    char* name;
    float x;
    float y;
    float velX;
    float velY;
    short int width;
    short int height;
    short int takingAction;
    short int takingActionCounter;
    short int maxTakingActionCounter;
    Direction direction;
    SDL_Rect col_up;
    SDL_Rect col_right;
    SDL_Rect col_down;
    SDL_Rect col_left;
} NPC;

typedef struct Ground {
    Vector2 vec;
    short int width;
    short int height;
    short int gid;
} Ground;

// ------------------ PUBLIC FUNCTIONS ------------------
Player* resetPlayer(char* name, float x, float y, short int width, short int height);
NPC* setNPC(int x, int y, int width, int height, Direction direction);
Ground* setGround(float x, float y, short int width, short int height);

void updateCamera(Camera* c, Player* player);
int getTileX(Player* p, unsigned int tw);
int getTileY(Player* p, unsigned int th);

int updateNPC(NPC* npc);
void updateCollisionsNPC(NPC* npc, Camera* cam);
void updateCollisionsPlayer(Player* p, Camera* cam);


#endif
