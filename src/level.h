#ifndef LEVEL_H_
#define LEVEL_H
#pragma once

#include <SDL2/SDL.h>


// ------------------ STRUCTS ------------------

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
    int textureNameCount;
    char* textureName[];
} Level;


// ------------------ PUBLIC FUNCTIONS ------------------

Level* getLevel();
void freeTiledMap(TiledMap* tiledMap);

#endif
