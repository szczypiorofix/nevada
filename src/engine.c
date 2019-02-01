#include <stdio.h>
#include <stdlib.h>
#include "engine.h"

// ------------------ FORWARD DECLARATION ------------------
struct Engine* createEngine(void);
bool initSDL(struct Engine* engine);
bool createWindow(struct Engine* engine);
bool createRenderer(struct Engine* engine);

struct Engine* engineStart();
void engineStop(struct Engine* engine);
void engineDelay(struct Engine* engine);





static Uint32 rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
static Uint32 windowFlags = SDL_WINDOW_SHOWN;



// ------------------ "PRIVATE" FUNCTIONS ------------------

struct Engine* createEngine(void) {
    struct Engine* engine = malloc(sizeof(engine));
    if (engine == NULL) return NULL;

    engine->started = FALSE;
    engine->quit = FALSE;
    engine->window = NULL;
    engine->renderer = NULL;
    engine->delayTime = 0.0f;
    engine->deltaTime = 0.0f;
    engine->startTick = 0L;
    engine->endTick = 0L;
    engine->fpsCounter = 0;
    engine->delayTime = 0.0f;
    engine->deltaTime = 0.0f;
    engine->fps = 0;
    return engine;
}

bool initSDL(struct Engine* engine) {
    engine->started = (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == 0);
    if (engine->started == FALSE) printf( "SDL_Init() Error: %s\n", SDL_GetError());
    return engine->started;
}

bool createWindow(struct Engine* engine) {
    engine->window = SDL_CreateWindow("Nevada", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);
    if (engine->window == NULL) {
        printf( "SDL_CreateWindow() Error: %s\n", SDL_GetError());
        engine->started = FALSE;
    }
    return (engine->window == NULL);
}


bool createRenderer(struct Engine* engine) {
    engine->renderer = SDL_CreateRenderer(engine->window, -1, rendererFlags);
    if (engine->renderer == NULL) {
        printf("SDL_CreateRenderer() Error: %s\n", SDL_GetError());
        engine->started = FALSE;
    } else {
        SDL_SetRenderDrawColor(engine->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    }
    return (engine->renderer == NULL);
}

bool initializePngImages(struct Engine* engine) {
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf( "IMG_Init() Error: %s\n", IMG_GetError());
        engine->started = FALSE;
    }
    return engine->started;
}

bool initializeAudioSystem(struct Engine* engine) {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf( "SDL_mixer Mix_OpenAudio() Error: %s\n", Mix_GetError() );
        engine->started = FALSE;
    }
    return engine->started;
}

bool initializeTTFFonts(struct Engine* engine) {
    if(TTF_Init() == -1) {
        printf( "SDL_ttf TTF_Init() Error: %s\n", TTF_GetError() );
        engine->started = FALSE;
    }
    return engine->started;
}


// ------------------ "PUBLIC" FUNCTIONS ------------------
struct Engine* engineStart(void) {
    struct Engine* engine = createEngine();

    initSDL(engine);
    createWindow(engine);
    createRenderer(engine);
    initializePngImages(engine);
    initializeAudioSystem(engine);
    initializeTTFFonts(engine);

    if (engine->started == FALSE) return NULL;
    return engine;
}

void engineDelay(struct Engine* engine) {
    float frameRate = (float) (1000.0f / ENGINE_FPS_MAX);
    if (engine->deltaTime < frameRate)
        SDL_Delay((int)(frameRate - engine->deltaTime));
}

void engineStop(struct Engine* engine) {
    engine->started = FALSE;

    SDL_DestroyRenderer(engine->renderer);
    SDL_DestroyWindow(engine->window);

    engine->window = NULL;
    engine->renderer = NULL;
	
	TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}


void updateDeltaTime(struct Engine* engine) {
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
