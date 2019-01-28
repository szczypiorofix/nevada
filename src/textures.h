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


void renderTexture(Texture* t, SDL_Game* game, SDL_Rect* clip, int x, int y, uint16 width, uint16 height);

Texture* loadSpriteSheet(const byte* fileName, SDL_Game* game, uint16 spriteWidth, uint16 spriteHeigth);

void freeTexture(Texture* t);

SDL_Rect* getSpriteI(Texture* t, int index, uint16 width, uint16 height);

SDL_Rect* createRectsForSprites(Level* level, int layerCount, const uint16 size, Texture* t);

Texture* loadFromRenderedText(const byte* textureText, SDL_Game* game);

void renderText(Texture* t, SDL_Game* game, int x, int y, int w, int h);
