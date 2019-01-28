#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "textures.h"



// RENDERER FLAGS
static Uint32 rendererFlags = SDL_RENDERER_ACCELERATED; // | SDL_RENDERER_PRESENTVSYNC;

static Uint32 windowFlags = SDL_WINDOW_SHOWN;



// FORWARD DECLARATION
SDL_Game* initGame();
Level* getLevel(short n);
void updateCamera(Camera* c, Player player);
int getTileX(Player* p, Camera* c, Level* l, unsigned int tw);
int getTileY(Player* p, Camera* c, Level* l, unsigned int t);
Animation* prepareAnimation(Texture* t, int speed, int sw, int sh, const int size, int* frames);
int nextFrame(Animation* an);



SDL_Game* initGame() {
    SDL_Game * game = malloc(sizeof(SDL_Game));
    if (game == NULL) return NULL;
    game->success = 1;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        game->success = 0;
    } else {
        game->gWindow = SDL_CreateWindow("Nevada", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);
        if (game->gWindow == NULL) {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError());
            game->success = 0;
        } else {
            game->gRenderer = SDL_CreateRenderer(game->gWindow, -1, rendererFlags);
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
        
    // parsing map
    TiledMap* tiledMap = parseMap("res/images/map.tmx");
    printf("TiledMap width: %i x height:%i, tileWidth:%i, tileHeight:%i\n", tiledMap->width, tiledMap->height, tiledMap->tileWidth, tiledMap->tileHeight);

    printf("TiledMap layers count: %i\n", tiledMap->layersCount);
    for (int i = 0; i < tiledMap->layersCount; i++) {
        printf("Layer info - id: %i, tilesCount: %i\n", tiledMap->layer[i].id, tiledMap->layer[i].data[0]);
        // for (int j = 0; j < tiledMap->layer[i].width * tiledMap->layer[i].height; j++) {
        //     printf("%i,", tiledMap->layer[i].data[j]);
        // }
    }

    Level* level = malloc(sizeof(Level));
    if (level == NULL) return NULL;

    const int w = 32;
    const int h = 16;

    level->width = w;
    level->height = h;
    level->size = w * h;
    
    level->content = malloc(sizeof *level->content * w * h);
    if (level->content == NULL) return NULL;

    level->width = tiledMap->layer[0].width;
    level->height = tiledMap->layer[0].height;
    level->content = tiledMap->layer;
    level->layers = tiledMap->layersCount;
    level->map = tiledMap;
    
    return level;
}


void updateCamera(Camera* c, Player player) {
    c->x = - player.x + (SCREEN_WIDTH / 2) - (player.width / 2);
    c->y = - player.y + (SCREEN_HEIGHT / 2) - (player.height / 2);
    c->offsetX = - player.x + (SCREEN_WIDTH / 2) - (player.width / 2);
    c->offsetY = - player.y + (SCREEN_HEIGHT / 2) - (player.height / 2);
}

int getTileX(Player* p, Camera* c, Level* l, unsigned int tw) {
    return ( (p->x + (p->width / 2)) / tw );
}

int getTileY(Player* p, Camera* c, Level* l, unsigned int th) {
    return ( (p->y + (p->height / 2)) / th );
}

Animation* prepareAnimation(Texture* t, int speed, int sw, int sh, const int size, int* frames) {
    Animation* anim = malloc(sizeof(Animation));
    if (anim == NULL) return NULL;

    anim->size = size;
    anim->speed = speed;

    SDL_Rect* r = malloc(sizeof(SDL_Rect) * size);
    if (r == NULL) return NULL;
    for (int i = 0; i < size; i++) {
        r[i] = *getSpriteI(t, frames[i], sw, sh);
    }
    anim->frames = r;
    return anim;
}

int nextFrame(Animation* an) {
    an->counter++;
    if (an->counter > an->speed) {
        an->counter = 0;
        an->curFrame++;
        if (an->curFrame >= an->size) {
            an->curFrame = 0;
        }
    }
    return an->curFrame;
}
