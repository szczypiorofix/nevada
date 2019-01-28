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
    short success;
    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
    Mix_Music *gMusic;
} SDL_Game;



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
} Player;


typedef struct NPC {
    int x;
    int y;
    int velX;
    int velY;
    int width;
    int height;
} NPC;


typedef struct Animation {
    int size;
    SDL_Rect* frames;
    int speed;
    int curFrame;
    int counter;
} Animation;


typedef struct Texture {
    SDL_Texture* mTexture;
    int width;
    int height;
    int sWidth;
    int sHeight;
} Texture;


typedef struct ListItem {
    int val;
    struct ListItem * next;
} ListItem;


typedef struct LinkedList {
    int size;
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
