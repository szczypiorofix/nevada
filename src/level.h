#ifndef LEVEL_H_
#define LEVEL_H
#pragma once


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
