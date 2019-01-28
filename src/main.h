#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "defines.h"
#include "level.h"



/**
 * #################################################
 * .................... STRUCTS ....................
 * #################################################
 * */

typedef struct SDL_Game {
    uint16 success;
    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
    Mix_Music *gMusic;
} SDL_Game;



typedef struct Camera {
    int32 x;
    int32 y;
    int32 offsetX;
    int32 offsetY;
} Camera;


typedef struct Player {
    int32 x;
    int32 y;
    int32 velX;
    int32 velY;
    uint16 width;
    uint16 height;
    int32 tileX;
    int32 tileY;
    uint32 tileIndex;
} Player;


typedef struct NPC {
    int32 x;
    int32 y;
    int32 velX;
    int32 velY;
    uint32 width;
    uint32 height;
} NPC;


typedef struct Animation {
    uint16 size;
    SDL_Rect* frames;
    uint16 speed;
    uint16 curFrame;
    uint32 counter;
} Animation;


typedef struct Texture {
    SDL_Texture* mTexture;
    uint16 width;
    uint16 height;
    uint16 sWidth;
    uint16 sHeight;
} Texture;


typedef struct ListItem {
    int val;
    struct ListItem * next;
} ListItem;


typedef struct LinkedList {
    uint32 size;
    ListItem* lastItem;
} LinkedList;





/**
 * #################################################
 * ................... FUNCTIONS ...................
 * #################################################
 * */


char* getCurrentTime();

void close(SDL_Game* g);

Player* resetPlayer(Texture* t);
