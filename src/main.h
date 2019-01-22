#ifndef MAIN_H_
#define MAIN_H_

#include "textures.h"
#include "game.h"


static const float SPEED = 2.0f;



char* getCurrentTime();

void close(SDL_Game* g);

Player* resetPlayer(Texture* t);


#endif