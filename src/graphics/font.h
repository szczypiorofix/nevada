#ifndef FONT_H_
#define FONT_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "../core/game.h"
#include "textures.h"


Texture* loadFromRenderedText(const char* textureText, SDL_Game* game);

void renderText(Texture* t, SDL_Game* game, int x, int y);


#endif
