#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "main.h"


/**
 * #################################################
 * ................... FUNCTIONS ...................
 * #################################################
 * */


void renderTexture(Texture* t, SDL_Game* game, SDL_Rect* clip, int x, int y, unsigned short width, unsigned short height);

Texture* loadSpriteSheet(const char* fileName, SDL_Game* game, unsigned short spriteWidth, unsigned short spriteHeigth);

void freeTexture(Texture* t);

SDL_Rect* getSpriteI(Texture* t, int index, unsigned short width, unsigned short height);

SDL_Rect* createRectsForSprites(Level* level, int layerCount, const unsigned short size, Texture* t);

Texture* loadFromRenderedText(const char* textureText, SDL_Game* game);

void renderText(Texture* t, SDL_Game* game, int x, int y, int w, int h);

