#ifndef TEXTURES_H_
#define TEXTURES_H_


#include "main.h"


void renderTexture(Texture* t, SDL_Game* game, SDL_Rect* clip, int x, int y, unsigned short width, unsigned short height);

Texture* loadSpriteSheet(const char* fileName, SDL_Game* game, unsigned short spriteWidth, unsigned short spriteHeigth);

void freeTexture(Texture* t);

SDL_Rect* getSpriteI(Texture* t, int index, unsigned short width, unsigned short height);

SDL_Rect* createRectsForSprites(Level* level, const unsigned short size, Texture* t);

Texture* loadFromRenderedText(const char* textureText, SDL_Game* game);

void renderText(Texture* t, SDL_Game* game, int x, int y, int w, int h);

#endif
