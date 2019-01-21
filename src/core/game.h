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

typedef struct Level
{
    int* content;
    unsigned short width;
    unsigned short height;
    unsigned int size;
} Level;


typedef struct Camera
{
    int x;
    int y;
    int offsetX;
    int offsetY;
} Camera;


Level* getLevel(short n);

void updateCamera(Camera* c, int x, int y, int offsetX, int offsetY);



SDL_Game* initGame();


#endif