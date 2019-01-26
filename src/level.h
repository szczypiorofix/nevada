#pragma once

#include <libxml/parser.h>

typedef struct Level {
    int* content;
    unsigned short width;
    unsigned short height;
    unsigned int size;
} Level;

typedef struct TileSetSourceImage {
    char* source;
    int width;
    int height;
} TileSetSourceImage;

typedef struct TileSetSource {
    char* name;
    int tileWidth;
    int tileHeight;
    int tileCount;
    int columns;
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
} TiledMap;


int xmlCharToInt(const xmlChar a[]);

TiledMap* parseMap(const char* fileName);
