#ifndef _FONT_H_
#define _FONT_H_
#pragma once


#include "engine.h"

// ------------------ STRUCTS ------------------

typedef struct BitmapFontChar {
    short ascii;
    short ucode;
    char raw;
    short bottom;
    short top;
    short x;
    short y;
    short width;
    short height;
    short trailing;
    short leading;
} BitmapFontChar;


typedef struct BitmapFont {
    short width;
    short height;
    short space;
    short itemsCount;
    BitmapFontChar* bitmapFontChar;
    SpriteSheet* bitmapFontImage;
} BitmapFont;



// ------------------ PUBLIC FUNCTIONS ------------------

BitmapFont* parseBitmapFile(char* pngFileName, char* xmlFileName, SDL_Renderer* renderer);

void drawBitmapFont(BitmapFont* bitmapFont, char * text, SDL_Renderer* renderer);

#endif
