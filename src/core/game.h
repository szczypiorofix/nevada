#ifndef GAME_H_
#define GAME_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>


//extern const int SCREEN_WIDTH;
//extern const int SCREEN_HEIGHT;



typedef struct SDL_Game {
    short success;
    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
    Mix_Music *gMusic;
} SDL_Game;


typedef struct Camera {
    int x;
    int y;
    int width;
    int height;
} Camera;


SDL_Game* initGame();


#endif