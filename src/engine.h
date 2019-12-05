#ifndef _ENGINE_H_
#define _ENGINE_H_
#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <libxml/parser.h>

#include "defines.h"

// ------------------- CONSTANTS -------------------

extern const short VSYNC_ON;
extern const short VSYNC_OFF;
extern const short TARGET_FPS;
extern const float OPTIMAL_TIME;

extern const short VIEW_LOCKED_ON_PLAYER;
extern const short VIEW_LOCKED_ON_MOUSE;



// ------------------- ENUMS -------------------

typedef enum {
    DIR_UP,
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT,
    DIR_UP_RIGHT,
    DIR_DOWN_RIGHT,
    DIR_DOWN_LEFT,
    DIR_UP_LEFT
} Direction;


typedef enum {
    SS_BACKGROUND,
    SS_PLAYER
} SpriteSheets;


typedef enum {
    ENGINE_WINDOW = 0x00000000,
    ENGINE_FULLSCREEN = 0x00000001
} WindowFullScreen;


// ------------------ STRUCTS ------------------


typedef struct SpriteSheet {
    SDL_Texture* mTexture;
    char* name;
    int width;
    int height;
    int tileWidth;
    int tileHeight;
} SpriteSheet;

typedef struct Animation {
    unsigned short size;
    SDL_Rect* frames;
    short speed;
    int curFrame;
    int counter;
    SpriteSheet* spriteSheet;
} Animation;


typedef struct Player {
    char* name;
    Vector2 vec;
    Vector2 moveVec;
    float speed;
    short int width;
    short int height;
    int tileX;
    int tileY;
    int tileIndex;
    short int isMoving;
    Direction direction;
    SDL_Rect col;
    Animation* walkingAnimation;
} Player;


typedef struct NPC {
    char* name;
    Vector2 vec;
    Vector2 moveVec;
    short int width;
    short int height;
    short int takingAction;
    short int takingActionCounter;
    short int maxTakingActionCounter;
    Direction direction;
    SDL_Rect col;
    Animation* walkingAnimation;
} NPC;


typedef struct Ground {
    Vector2 vec;
    short int width;
    short int height;
    short int gid;
} Ground;


typedef struct Camera {
    Vector2 vec;
} Camera;


typedef struct TextFont {
	char* text;
	SDL_Color textColor;
	TTF_Font* font;
	SpriteSheet* texture;
} TextFont;


typedef struct Engine {
    int started;
    int quit;
    float scale;
    float minScale;
    float maxScale;
    int tilesOnScreenFromCenterX;
    int tilesOnScreenFromCenterY;
    SDL_Event event;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Mix_Music *music;
    Camera* camera;

    Vector2* scrollVector;
    float* lockCameraOnObjectX;
    float* lockCameraOnObjectY;

    short fullScreen;
    short viewLockedOn;
    int musicVolume;
    long lastTime;
	double delta;
	long timer;
	int updates;
	int frames;
	long now;
	float amountOfTicks;
	int fps_count;
	int ticks_count;
	double ns;
    short int fpsCap;
    int displayMode;
    TextFont* coordinates;
    char coordinatesText[80];
    short mouseRightButtonPressed;
} Engine;




typedef struct TileSetSource {
    char* name;
    int tileWidth;
    int tileHeight;
    int tileCount;
    int columns;
    int width;
    int height;
    char* imageSource;
} TileSetSource;

typedef struct TileSet {
    int firstGid;
    char* source;
    TileSetSource* tileSetSource;
} TileSet;

typedef struct Layer {
    int id;
    char* name;
    int width;
    int height;
    char* encoding;
    int (*data);
} Layer;

typedef struct TiledObject {
    int id;
    char* name;
    char* type;
    char* template;
    int x;
    int y;
    int firstGid;
    char* source;
    int gid;
    int width;
    int height;
} TiledObject;

typedef struct ObjectGroup {
    int id;
    char* name;
    int objectsCount;
    TiledObject* objects;
} ObjectGroup;

typedef struct TiledMap {
    int width;
    int height;
    int tileWidth;
    int tileHeight;
    int nextLayerId;
    int nextObjectGid;
    int layersCount;
    int tileSetCount;
    int objectGroupCount;
    TileSet* tileSet;
    Layer* layer;
    ObjectGroup* ObjectGroup;
} TiledMap;


typedef struct Level {
    Layer* content;
    unsigned short layers;
    unsigned short width;
    unsigned short height;
    unsigned int size;
    TiledMap* map;
    unsigned short textureNameCount;
    unsigned short columns;
    char* textureName[];
} Level;


// ------------------ BASIC STRUCTURES ------------------
Engine* engine;


// ------------------ PUBLIC FUNCTIONS ------------------

Engine* engineStart(void);
void engineStop(Engine** engine);
int loadMusic(char* musicFile);
void updateCamera();
void lockCameraOnObject(float* x, float* y);
int setFullScreen(WindowFullScreen flag);
SpriteSheet* loadSpriteSheet(char* fileName, SDL_Renderer* renderer, unsigned int spriteWidth, unsigned int spriteHeigth);
void freeTexture(SpriteSheet* t);
void renderTexture(SpriteSheet* t, SDL_Renderer* renderer, SDL_Rect* clip, int x, int y, float scale, double angle, SDL_Point* center, SDL_RendererFlip flip, int mode);
SDL_Rect* createRectsForSprites(Level* level, unsigned short layerCount, SpriteSheet* t);
Animation* prepareAnimation(SpriteSheet* t, unsigned int speed, unsigned int sw, unsigned int sh, const unsigned short size, const unsigned int* frames);
int nextFrame(Animation* a);
int checkCollision(SDL_Rect r1, SDL_Rect r2);
Player* resetPlayer(char* name, float x, float y, short width, short height);
NPC* setNPC(int x, int y, int width, int height, Direction direction);
Ground* setGround(float x, float y, short width, short height);
int getTileX(float x, unsigned int width, unsigned int tileWidth);
int getTileY(float y, unsigned int height, unsigned int tileHeight);
int updateNPC(NPC* npc, Level* level);
void updateCollisionsNPC(NPC* npc, const Camera* cam, const float scale);
void updateCollisionsPlayer(Player* p, const Camera* cam, const float scale);
void drawNPCCollisions(NPC* npc, SDL_Renderer* renderer);
Level* getLevel(char* fileName);
void freeTiledMap(TiledMap* tiledMap);
TextFont* loadFromRenderedText(const char* textureText, SDL_Renderer* renderer);
void renderText(TextFont* t, SDL_Renderer* renderer, int x, int y, int w, int h);
void changeText(TextFont* t, SDL_Renderer* renderer, char* text);
int xmlCharToInt(const xmlChar a[]);
int stringToInt(const char a[]);

#endif
