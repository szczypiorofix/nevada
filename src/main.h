#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "defines.h"
#include "level.h"




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








/**
 * #################################################
 * ................... FUNCTIONS ...................
 * #################################################
 * */


char* getCurrentTime(void);

void close(SDL_Game* g);

Player* resetPlayer();

void updateCamera(Camera* c, Player player);

SDL_Game* initGame(void);

int getTileX(Player* p, unsigned int tw);

int getTileY(Player* p, unsigned int th);

// Animation* prepareAnimation(Texture* t, unsigned int speed, unsigned int sw, unsigned int sh, const unsigned int size, unsigned int* frames);

// int nextFrame(Animation* an);

// void renderTexture(Texture* t, SDL_Game* game, SDL_Rect* clip, int x, int y, unsigned int width, unsigned int height);

// Texture* loadSpriteSheet(char* fileName, SDL_Game* game, unsigned int spriteWidth, unsigned int spriteHeigth);

// void freeTexture(Texture* t);

// SDL_Rect* getSpriteI(Texture* t, int index, unsigned int width, unsigned int height);

// SDL_Rect* createRectsForSprites(Level* level, int layerCount, const unsigned int size, Texture* t);

// // Texture* loadFromRenderedText(const char* textureText, SDL_Game* game);

// void renderText(Texture* t, SDL_Game* game, int x, int y, int w, int h);
