#ifndef _ENGINE_H_
#define _ENGINE_H_
#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <libxml/parser.h>

#include "defines.h"


#define VSYNC_ON  1
#define VSYNC_OFF 0
#define TARGET_FPS = 60.0f;
#define OPTIMAL_TIME = 1000.0f / TARGET_FPS;


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
    short mouseX;
    short mouseY;
    TextFont* coordinates;
    char coordinatesText[35];
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
void updateCamera(const Player* player, const Level* level);
SpriteSheet* loadSpriteSheet(char* fileName, SDL_Renderer* renderer, unsigned int spriteWidth, unsigned int spriteHeigth);
void freeTexture(SpriteSheet* t);
void renderTexture(SpriteSheet* t, SDL_Renderer* renderer, SDL_Rect* clip, int x, int y, float scale, double angle, SDL_Point* center, SDL_RendererFlip flip, int mode);
SDL_Rect* createRectsForSprites(Level* level, unsigned short layerCount, SpriteSheet* t);
Animation* prepareAnimation(SpriteSheet* t, unsigned int speed, unsigned int sw, unsigned int sh, const unsigned short size, const unsigned int* frames);
int nextFrame(Animation* a);
int releaseAnimation(Animation** a);
int checkCollision(SDL_Rect r1, SDL_Rect r2);
Player* resetPlayer(char* name, float x, float y, short width, short height);
NPC* setNPC(int x, int y, int width, int height, Direction direction);
Ground* setGround(float x, float y, short width, short height);
int getTileX(Player* p, unsigned int tileWidth);
int getTileY(Player* p, unsigned int tileHeight);
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
