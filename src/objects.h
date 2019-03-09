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
    Vector2 moveVec;
    short int width;
    short int height;
    int tileX;
    int tileY;
    int tileIndex;
    short int isMoving;
    Direction direction;
    SDL_Rect col;
} Player;


typedef struct NPC {
    char* name;
    Vector2 vec;
    Vector2 moveVec;
    short int width;
    short int height;
    short int takingAction;
    short int takingActionCounter;
    short int maxTakingActionCounter;
    Direction direction;
    SDL_Rect col;
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

void updateCamera(Camera* c, Player* player, Level* level, int scale);
int getTileX(Player* p, unsigned int tileWidth);
int getTileY(Player* p, unsigned int tileHeight);

int updateNPC(NPC* npc, Level* level);
void updateCollisionsNPC(NPC* npc, Camera* cam, int scale);
void updateCollisionsPlayer(Player* p, Camera* cam, int scale);

void drawNPCCollisions(NPC* npc, SDL_Renderer* renderer);


#endif
