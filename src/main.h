#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "defines.h"



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
    int width;
    int height;
    int tileX;
    int tileY;
    int tileIndex;
} Player;


typedef struct NPC {
    int x;
    int y;
    int velX;
    int velY;
    int width;
    int height;
} NPC;


typedef struct Animation {
    int size;
    SDL_Rect* frames;
    int speed;
    int curFrame;
    int counter;
} Animation;


typedef struct Texture {
    SDL_Texture* mTexture;
    int width;
    int height;
    int sWidth;
    int sHeight;
} Texture;


typedef struct ListItem {
    int val;
    struct ListItem * next;
} ListItem;


typedef struct LinkedList {
    int size;
    ListItem* lastItem;
} LinkedList;


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
    int* data;
} Layer;

typedef struct TiledMap {
    int width;
    int height;
    int tileWidth;
    int tileHeight;
    int nextLayerId;
    int nextObjectGid;
    TileSet* tileSet;
    Layer* layer;
    int layersCount;
    int tileSetCount;
} TiledMap;


typedef struct Level {
    Layer* content;
    int layers;
    unsigned short width;
    unsigned short height;
    unsigned int size;
    TiledMap* map;
} Level;

typedef enum {
	true,
	false
} Boolean;


typedef struct {
	int size;
	char* content;
	char* name;
	Boolean success;
} BinaryFile;




/**
 * #################################################
 * ................... FUNCTIONS ...................
 * #################################################
 * */


char* getCurrentTime(void);

void close(SDL_Game* g);

Player* resetPlayer(Texture* t);

Level* getLevel(short n);

void updateCamera(Camera* c, Player player);

SDL_Game* initGame(void);

int getTileX(Player* p, Camera* c, Level* l, unsigned int tw);

int getTileY(Player* p, Camera* c, Level* l, unsigned int th);

Animation* prepareAnimation(Texture* t, unsigned int speed, unsigned int sw, unsigned int sh, const unsigned int size, unsigned int* frames);

int nextFrame(Animation* an);

void renderTexture(Texture* t, SDL_Game* game, SDL_Rect* clip, int x, int y, unsigned int width, unsigned int height);

Texture* loadSpriteSheet(char* fileName, SDL_Game* game, unsigned int spriteWidth, unsigned int spriteHeigth);

void freeTexture(Texture* t);

SDL_Rect* getSpriteI(Texture* t, int index, unsigned int width, unsigned int height);

SDL_Rect* createRectsForSprites(Level* level, int layerCount, const unsigned int size, Texture* t);

// Texture* loadFromRenderedText(const char* textureText, SDL_Game* game);

void renderText(Texture* t, SDL_Game* game, int x, int y, int w, int h);

int xmlCharToInt(const xmlChar a[]);

int* parseData(xmlDocPtr doc, xmlNodePtr cur);

TiledMap* parseMap(const char* fileName);

void freeTiledMap(TiledMap* tiledMap);

int xmlCharToInt(const xmlChar a[]);

TiledMap* parseMap(const char* fileName);

void freeTiledMap(TiledMap* tiledMap);

char* copyStringFromPointer(const char* s);

int fromBinary(const char *s);
