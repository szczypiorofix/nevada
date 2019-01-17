#ifndef GAME_H_
#define GAME_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;


typedef struct SDL_Game {
    short success;
    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
    Mix_Music *gMusic;
} SDL_Game;


SDL_Game* initGame();


#endif