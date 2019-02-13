#include <stdio.h>
#include <stdlib.h>

#include "engine.h"

// ------------------ FORWARD DECLARATION ------------------
Engine* createEngine(void);
bool initSDL(Engine* engine);
bool createWindow(Engine* engine);
bool createRenderer(Engine* engine);

struct Engine* engineStart();
void engineStop(Engine** engine);
void engineDelay(Engine* engine);

AssetsList* createAssets(void);
int loadMusic(Engine* engine, char* musicFile);


// ------------------ "PRIVATE" FUNCTIONS ------------------

Engine* createEngine(void) {
    Engine* engine = malloc(sizeof(Engine));
    if (engine == NULL) return NULL;
    
    engine->started = FALSE;
    engine->quit = FALSE;
    engine->window = NULL;
    engine->renderer = NULL;
    engine->music = NULL;
    engine->musicVolume = 20;
    engine->delayTime = 0.0f;
    engine->deltaTime = 0.0f;
    engine->startTick = 0L;
    engine->endTick = 0L;
    engine->fpsCounter = 0;
    engine->delayTime = 0.0f;
    engine->deltaTime = 0.0f;
    engine->fps = 0;
    engine->assets = NULL;

    return engine;
}

bool initSDL(Engine* engine) {
    engine->started = (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == 0);
    if (engine->started == FALSE) printf( "SDL_Init() Error: %s\n", SDL_GetError());
    atexit(SDL_Quit);
    return engine->started;
}

bool createWindow(Engine* engine) {
    engine->window = SDL_CreateWindow("Nevada", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (engine->window == NULL) {
        printf( "SDL_CreateWindow() Error: %s\n", SDL_GetError());
        engine->started = FALSE;
    }
    return (engine->window == NULL);
}


bool createRenderer(Engine* engine) {
    engine->renderer = SDL_CreateRenderer(engine->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (engine->renderer == NULL) {
        printf("SDL_CreateRenderer() Error: %s\n", SDL_GetError());
        engine->started = FALSE;
    } else {
        SDL_SetRenderDrawColor(engine->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    }
    return (engine->renderer == NULL);
}

bool initializePngImages(Engine* engine) {
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf( "IMG_Init() Error: %s\n", IMG_GetError());
        engine->started = FALSE;
    }
    return engine->started;
}

bool initializeAudioSystem(Engine* engine) {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer Mix_OpenAudio() Error: %s\n", Mix_GetError());
        engine->started = FALSE;
    }
    return engine->started;
}

bool initializeTTFFonts(Engine* engine) {
    if(TTF_Init() == -1) {
        printf( "SDL_ttf TTF_Init() Error: %s\n", TTF_GetError() );
        engine->started = FALSE;
    }
    return engine->started;
}

AssetsList* createAssets(void) {
    printf("Preparing assets...\n");
    AssetsList* list = createAssetsList(5, 3, sizeof(Texture), ASSETSLIST_SHRINK_AFTER_DELETE);
    if (!list) {
        fprintf(stderr, "ERROR !!! Unable to create AssetsList !!!\n");
        return NULL;
    }
    return list;
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

    engine->assets = createAssets();
    
    if (engine->started == FALSE || engine->assets == NULL) return NULL;
   
    return engine;
}

void engineDelay(Engine* engine) {
    float frameRate = (float) (1000.0f / ENGINE_FPS_MAX);
    if (engine->deltaTime < frameRate)
        SDL_Delay((int)(frameRate - engine->deltaTime));
}

void engineStop(Engine** engine) {
    printf("Releasing assets ...\n");
    clearAssetsList( &(*engine)->assets );

    (*engine)->started = FALSE;

    Mix_FreeMusic((*engine)->music);
    (*engine)->music = NULL;

    SDL_DestroyRenderer((*engine)->renderer);
    (*engine)->renderer = NULL;

    SDL_DestroyWindow((*engine)->window);
    (*engine)->window = NULL;
    
    printf("Free engine...\n");
	
    free(*engine);
    (*engine) = NULL;

	TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}


void updateDeltaTime(Engine* engine) {
    engine->startTick = SDL_GetPerformanceCounter();
    engine->deltaTime = (float)(engine->startTick - engine->endTick) / SDL_GetPerformanceFrequency();
    engine->endTick = engine->startTick;
    engine->fpsCounter++;
    engine->delayTime += engine->deltaTime;
    if (engine->delayTime >= 1.0f) {
        engine->delayTime -= 1.0f;
        engine->fps = engine->fpsCounter;
        engine->fpsCounter = 0;
    }
}

int loadMusic(Engine* engine, char* musicFile) {
    engine->music = Mix_LoadMUS(musicFile);
    if (engine->music == NULL) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        return 0;
    }
    Mix_VolumeMusic(engine->musicVolume);
    return 1;
}