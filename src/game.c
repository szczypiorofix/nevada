#include "game.h"
#include <stdio.h>
#include <stdlib.h>


// FORWARD DECLARATION

SDL_Game* initGame();
Level* getLevel(short n);
void updateCamera(Camera* c, Player player);

int getTileX(Player* p, Camera* c, Level* l);
int getTileY(Player* p, Camera* c, Level* l);





SDL_Game* initGame() {
    SDL_Game * game = malloc(sizeof(SDL_Game));
    game->success = 1;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        game->success = 0;
    } else {
        game->gWindow = SDL_CreateWindow("Nevada", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (game->gWindow == NULL) {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError());
            game->success = 0;
        } else {
            game->gRenderer = SDL_CreateRenderer(game->gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (game->gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                game->success = 0;
            } else {
                SDL_SetRenderDrawColor(game->gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                
                int imgFlags = IMG_INIT_PNG;
                // INITIALIZE IMGs
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    game->success = 0;
                }

                // INITIALIZE AUDIO
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    game->success = 0;
                }

                // INITIALIZE TTF
                if(TTF_Init() == -1) {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    game->success = 0;
                }
            }
        }
    }
    game->gMusic = Mix_LoadMUS("res/ex-aws_cave.xm");
    if (game->gMusic == NULL) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        game->success = 0;
    }
    return game;
}



Level* getLevel(short n) {
    Level* level = malloc(sizeof(Level));
    const int w = 14;
    const int h = 10;

    level->width = w;
    level->height = h;
    level->size = w * h;
    
    level->content = malloc(sizeof *level->content * w * h);
    // 14 x 10
    int l[] = {
			17, 19, 18, 17, 17, 17, 17, 17, 18, 17, 17, 17, 17 ,17,
			17, 17, 19, 17, 17, 17, 17, 18, 17, 17, 17, 17, 17 ,17,
            17, 18, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17 ,17,
            17, 17, 17, 17, 19, 17, 17, 17, 17, 17, 17, 17, 17 ,17,
            17, 17, 19, 17, 17, 17, 17, 19, 17, 17, 17, 17, 17 ,17,
            17, 18, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17 ,17,
            17, 17, 19, 17, 17, 17, 17, 18, 17, 17, 17, 17, 17 ,17,
            17, 17, 17, 17, 19, 17, 17, 17, 17, 17, 17, 17, 17 ,17,
            17, 17, 17, 17, 19, 17, 17, 17, 17, 17, 17, 17, 17 ,17,
            17, 17, 17, 17, 17, 18, 17, 17, 17, 17, 17, 17, 17 ,17
	};
    
    for (int i = 0; i < w * h; i++) {
        level->content[i] = *(&l[i]);
    }
    
    return level;
}


void updateCamera(Camera* c, Player player) {
    c->x = - player.x + (SCREEN_WIDTH / 2) - (player.width / 2);
    c->y = - player.y + (SCREEN_HEIGHT / 2) - (player.height / 2);
    c->offsetX = - player.x + (SCREEN_WIDTH / 2) - (player.width / 2);
    c->offsetY = - player.y + (SCREEN_HEIGHT / 2) - (player.height / 2);
}

int getTileX(Player* p, Camera* c, Level* l) {
    return ((p->x / l->width) + c->offsetX) / 64;
}

int getTileY(Player* p, Camera* c, Level* l) {
    return ((p->y / l->height) + c->offsetY) / 64;
}