#include <stdio.h>
#include <stdlib.h>

#include "engine.h"


const int TARGET_FPS = 60;
const long OPTIMAL_TIME = 1000 / TARGET_FPS;


// ------------------ FORWARD DECLARATION ------------------
Engine* createEngine(void);
int initSDL(Engine* engine);
int createWindow(Engine* engine);
int createRenderer(Engine* engine);

struct Engine* engineStart();
void engineStop(Engine** engine);
void engineDelay(Engine* engine);
int loadMusic(Engine* engine, char* musicFile);


// ------------------ "PRIVATE" FUNCTIONS ------------------

Engine* createEngine(void) {
    Engine* engine = malloc(sizeof(Engine));
    if (engine == NULL) return NULL;
    
    engine->started = 0;
    engine->quit = 0;
    engine->window = NULL;
    engine->renderer = NULL;
    engine->music = NULL;
    engine->musicVolume = 20;
    engine->scale = 2;
    engine->minScale = 1;
    engine->maxScale = 5;
    engine->tilesOnScreenFromCenterX = 0;
    engine->tilesOnScreenFromCenterY = 0;
    
    // GAME LOOP
    engine->fpsCap = 0;
    engine->lastTime = SDL_GetTicks();
	engine->delta = 0.0f;
	engine->timer = SDL_GetTicks();
	engine->updates = 0;
	engine->frames = 0;
	engine->now = 0L;
	engine->amountOfTicks = 60.0f;
	engine->fps_count = 0;
	engine->ticks_count = 0;
	engine->ns = 0;

    engine->displayMode = 0;

    return engine;
}

int initSDL(Engine* engine) {
    engine->started = (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == 0);
    if (!engine->started) printf( "SDL_Init() Error: %s\n", SDL_GetError());
    atexit(SDL_Quit);
    return engine->started;
}

int createWindow(Engine* engine) {
    engine->window = SDL_CreateWindow("Nevada", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (engine->window == NULL) {
        printf( "SDL_CreateWindow() Error: %s\n", SDL_GetError());
        engine->started = 0;
    }
    return (engine->window == NULL);
}


int createRenderer(Engine* engine) {
    engine->renderer = SDL_CreateRenderer(engine->window, -1, SDL_RENDERER_ACCELERATED );// | SDL_RENDERER_PRESENTVSYNC);
    if (engine->renderer == NULL) {
        printf("SDL_CreateRenderer() Error: %s\n", SDL_GetError());
        engine->started = 0;
    } else {
        SDL_SetRenderDrawColor(engine->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    }
    return (engine->renderer == NULL);
}

int initializePngImages(Engine* engine) {
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf( "IMG_Init() Error: %s\n", IMG_GetError());
        engine->started = 0;
    }
    return engine->started;
}

int initializeAudioSystem(Engine* engine) {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer Mix_OpenAudio() Error: %s\n", Mix_GetError());
        engine->started = 0;
    }
    return engine->started;
}

int initializeTTFFonts(Engine* engine) {
    if(TTF_Init() == -1) {
        printf( "SDL_ttf TTF_Init() Error: %s\n", TTF_GetError() );
        engine->started = 0;
    }
    return engine->started;
}


// ------------------ "PUBLIC" FUNCTIONS ------------------
Engine* engineStart(void) {
    Engine* engine = createEngine();
    initSDL(engine);
    createWindow(engine);
    createRenderer(engine);
    initializePngImages(engine);
    initializeAudioSystem(engine);
    initializeTTFFonts(engine);

    // engine->assets = createAssets();
    
    if (engine->started == 0) return NULL;
   
    return engine;
}


void engineStop(Engine** engine) {
    // clearAssetsList( &(*engine)->assets );

    (*engine)->started = 0;

    Mix_FreeMusic((*engine)->music);
    (*engine)->music = NULL;

    SDL_DestroyRenderer((*engine)->renderer);
    (*engine)->renderer = NULL;

    SDL_DestroyWindow((*engine)->window);
    (*engine)->window = NULL;
    	
    free(*engine);
    (*engine) = NULL;

	TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}


// void updateDeltaTime(Engine* engine) {
//     engine->startTick = SDL_GetPerformanceCounter();
//     engine->deltaTime = (float)(engine->startTick - engine->endTick) / SDL_GetPerformanceFrequency();
//     engine->endTick = engine->startTick;
//     engine->fpsCounter++;
//     engine->delayTime += engine->deltaTime;
//     if (engine->delayTime >= 1.0f) {
//         engine->delayTime -= 1.0f;
//         engine->fps = engine->fpsCounter;
//         engine->fpsCounter = 0;
//     }
// }


// void engineDelay(Engine* engine) {
    
// }

int loadMusic(Engine* engine, char* musicFile) {
    engine->music = Mix_LoadMUS(musicFile);
    if (engine->music == NULL) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        return 0;
    }
    Mix_VolumeMusic(engine->musicVolume);
    return 1;
}