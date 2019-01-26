#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "level.h"

// Booleans
#define FALSE 0
#define TRUE  1
typedef unsigned char bool;


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

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
    unsigned int width;
    unsigned int height;
    int tileX;
    int tileY;
    int tileIndex;
} Player;


typedef struct NPC {
    int x;
    int y;
    int velX;
    int velY;
    unsigned int width;
    unsigned int height;
} NPC;


typedef struct Animation {
    unsigned short size;
    SDL_Rect* frames;
    unsigned short speed;
    unsigned short curFrame;
    unsigned int counter;
} Animation;


typedef struct Texture {
    SDL_Texture* mTexture;
    unsigned short width;
    unsigned short height;
    unsigned short sWidth;
    unsigned short sHeight;
} Texture;


typedef struct ListItem {
    int val;
    struct ListItem * next;
} ListItem;


typedef struct LinkedList {
    unsigned int size;
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

