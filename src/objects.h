#ifndef OBJECTS_H_
#define OBJECTS_H_
#pragma once



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



Player* resetPlayer(void);
void updateCamera(Camera* c, Player player);
int getTileX(Player* p, unsigned int tw);
int getTileY(Player* p, unsigned int th);


#endif
