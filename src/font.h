#ifndef FONT_H_
#define FONT_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "game.h"
#include "textures.h"


Texture* loadFromRenderedText(const char* textureText, SDL_Game* game);

void renderText(Texture* t, SDL_Game* game, int x, int y);


#endif
