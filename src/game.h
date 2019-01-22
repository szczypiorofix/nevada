#ifndef GAME_H_
#define GAME_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

/**
 * #################################################
 * ................... CONSTANTS ...................
 * #################################################
 * */
static const int SCREEN_WIDTH = 800;
static const int SCREEN_HEIGHT = 600;
static const int WALK_LEFT = 0;
static const int WALK_RIGHT = 1;
static const int WALK_UP = 2;
static const int WALK_DOWN = 3;

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
    float x;
    float y;
    float offsetX;
    float offsetY;
} Camera;

typedef struct Player {
    float x;
    float y;
    float velX;
    float velY;
    unsigned int width;
    unsigned int height;
    int tileX;
    int tileY;
} Player;



/**
 * #################################################
 * ................... FUNCTIONS ...................
 * #################################################
 * */

Level* getLevel(short n);

void updateCamera(Camera* c, Player player);

SDL_Game* initGame();

// Player* resetPlayer(Texture* t) {
//     Player* p = malloc(sizeof(Player));
//     p->x = (float) (SCREEN_WIDTH / 2);
//     p->y = (float) (SCREEN_HEIGHT / 2);
//     p->velX = 0.0f;
//     p->velY = 0.0f;
//     //p->width = t->sW
//     return p;
// }

int getTileX(Player* p, Camera* c, Level* l);

int getTileY(Player* p, Camera* c, Level* l);


#endif