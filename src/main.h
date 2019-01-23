#ifndef MAIN_H_
#define MAIN_H_


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>



/**
 * #################################################
 * ................... CONSTANTS ...................
 * #################################################
 * */
static const short SCREEN_WIDTH = 800;
static const short SCREEN_HEIGHT = 600;
static const short WALK_LEFT = 0;
static const short WALK_RIGHT = 1;
static const short WALK_UP = 2;
static const short WALK_DOWN = 3;
static const short SPEED = 2;


/**
 * #################################################
 * .................... STRUCTS ....................
 * #################################################
 * */

typedef struct SDL_Game {
    short success;
    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
    Mix_Music *gMusic;
} SDL_Game;

typedef struct Level {
    int* content;
    unsigned short width;
    unsigned short height;
    unsigned int size;
} Level;


typedef struct Camera {
    int x;
    int y;
    int offsetX;
    int offsetY;
} Camera;

typedef struct Player {
    int x;
    int y;
    int velX;
    int velY;
    unsigned int width;
    unsigned int height;
    int tileX;
    int tileY;
    int tileIndex;
} Player;


typedef struct NPC {
    int x;
    int y;
    int velX;
    int velY;
    unsigned int width;
    unsigned int height;
    int tileX;
    int tileY;
    int tileIndex;
} NPC;


typedef struct Animation {
    unsigned short size;
    SDL_Rect* frames;
    unsigned short speed;
} Animation;


typedef struct Texture
{
    SDL_Texture* mTexture;
    unsigned short width;
    unsigned short height;
    unsigned short sWidth;
    unsigned short sHeight;
} Texture;








char* getCurrentTime();

void close(SDL_Game* g);

Player* resetPlayer(Texture* t);


#endif
