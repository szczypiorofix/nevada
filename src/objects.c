#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "level.h"
#include "objects.h"



// ------------------ FORWARD DECLARATION ------------------
Player* resetPlayer(char* name, float x, float y, short int width, short int height);
void updateCamera(Camera* c, Player* player, Level* level, int scale);
int getTileX(Player* p, unsigned int tileWith);
int getTileY(Player* p, unsigned int tileHeight);
int updateNPC(NPC* npc, Level* level);
int random(int min, int max);
NPC* setNPC(int x, int y, int width, int height, Direction direction);
void updateCollisionsNPC(NPC* npc, Camera* cam, int scale);
Ground* setGround(float x, float y, short int width, short int height);
void updateCollisionsPlayer(Player* p, Camera* cam, int scale);
void drawNPCCollisions(NPC* npc, SDL_Renderer* renderer);


// ------------------ PUBLIC FUNCTIONS ------------------
Player* resetPlayer(char* name, float x, float y, short int width, short int height) {
    Player* p = malloc(sizeof(Player));
	if (p == NULL) {
        fprintf(stderr, "Malloc error while creating Player!!!\n");
        return NULL;
    }
    
    p->name = name;
    p->width = width;
	p->height = height;
    p->direction = DIR_RIGHT;
    
    p->vec = setVector(x, y);
    p->moveVec = setVector(0, 0);

    p->isMoving = 0;
    SDL_Rect c = { p->vec.x, p->vec.y, p->width, p->height };
    p->col = c;
    return p;
}


void updateCamera(Camera* c, Player* player, Level* level, int scale) {

    // if ( (player->vec.x * scale) - (SCREEN_WIDTH / 2)  > 0 
    //     && (player->vec.x * scale) < (level->width * 16)
    // )   
        c->vec.x = (player->vec.x * scale) - (SCREEN_WIDTH / 2);
    
    // if ( (player->vec.y * scale) - (SCREEN_HEIGHT / 2) > 0 

    // )    
        c->vec.y = (player->vec.y * scale) - (SCREEN_HEIGHT / 2);
}


int getTileX(Player* p, unsigned int tileWidth) {
    return ( (p->vec.x + (p->width / 2)) / tileWidth );
}


int getTileY(Player* p, unsigned int tileHeight) {
    return ( (p->vec.y + (p->height / 2)) / tileHeight );
}


void updateCollisionsNPC(NPC* npc, Camera* cam, int scale) {
    SDL_Rect c = { 
        ( (npc->vec.x - (npc->width / 2)) * scale) - cam->vec.x,
        ( (npc->vec.y - (npc->height / 2)) * scale) - cam->vec.y,
        npc->width * scale,
        npc->height * scale,
    };
    npc->col = c;
}


void updateCollisionsPlayer(Player* p, Camera* cam, int scale) {
    SDL_Rect c = { 
        ( (p->vec.x - (p->width / 2)) * scale) - cam->vec.x,
        ( (p->vec.y - (p->height / 2)) * scale) - cam->vec.y,
        p->width * scale,
        p->height * scale,
    };
    p->col = c;
}


int randomNPCActivity_move() {
    return random(60, 160);
}


int randomNPCActivity_wait() {
    return random(20, 150);
}


int updateNPC(NPC* npc, Level* level) {

    // COUNTING ACTIONS TIME
    if (npc->takingActionCounter < npc->maxTakingActionCounter)
        npc->takingActionCounter++;
    else {
        npc->takingActionCounter = 0;
        npc->maxTakingActionCounter = 0;
    }
    
    if (npc->takingActionCounter == 0) { // ANOTHER RANDOM ACTION
        if (npc->takingAction == 0) {
            switch (random(0, 8)) {
            // Basic directions
            case DIR_UP:
                npc->moveVec.x = 0;
                npc->moveVec.y = -NPC_SPEED;
                npc->direction = DIR_UP;
                npc->maxTakingActionCounter = randomNPCActivity_move();
                break;
            case DIR_RIGHT:
                npc->moveVec.x = NPC_SPEED;
                npc->moveVec.y = 0;
                npc->direction = DIR_RIGHT;
                npc->maxTakingActionCounter = randomNPCActivity_move();
                break;
            case DIR_DOWN:
                npc->moveVec.x = 0;
                npc->moveVec.y = NPC_SPEED;
                npc->direction = DIR_DOWN;
                npc->maxTakingActionCounter = randomNPCActivity_move();
                break;
            case DIR_LEFT:
                npc->moveVec.x = -NPC_SPEED;
                npc->moveVec.y = 0;
                npc->direction = DIR_LEFT;
                npc->maxTakingActionCounter = randomNPCActivity_move();
                break;
            // Additional directions
            case DIR_UP_RIGHT:
                npc->moveVec.x = NPC_SPEED;
                npc->moveVec.y = -NPC_SPEED;
                npc->direction = DIR_RIGHT;
                npc->maxTakingActionCounter = randomNPCActivity_move();
                break;
            case DIR_DOWN_RIGHT:
                npc->moveVec.x = NPC_SPEED;
                npc->moveVec.y = NPC_SPEED;
                npc->direction = DIR_RIGHT;
                npc->maxTakingActionCounter = randomNPCActivity_move();
                break;
            case DIR_DOWN_LEFT:
                npc->moveVec.x = -NPC_SPEED;
                npc->moveVec.y = NPC_SPEED;
                npc->direction = DIR_LEFT;
                npc->maxTakingActionCounter = randomNPCActivity_move();
                break;
            case DIR_UP_LEFT:
                npc->moveVec.x = -NPC_SPEED;
                npc->moveVec.y = -NPC_SPEED;
                npc->direction = DIR_LEFT;
                npc->maxTakingActionCounter = randomNPCActivity_move();
                break;
            }
            npc->takingAction = 1;
        } else {
            npc->takingAction = 0;
            npc->takingActionCounter = 0;
            npc->maxTakingActionCounter = randomNPCActivity_wait();
            npc->moveVec.x = 0;
            npc->moveVec.y = 0;
        }  
    }

    // addVector(&npc->vec, &npc->moveVec);
    if (npc->vec.x < npc->width / 2)
        npc->vec.x = npc->width / 2;
    if (npc->vec.y < npc->height / 2)
        npc->vec.y = npc->height / 2;
    if (npc->vec.x > (level->width * level->map->tileWidth) - npc->width)
        npc->vec.x = (level->width * level->map->tileWidth) - npc->width;
    if (npc->vec.y > (level->height * level->map->tileHeight) - npc->height)
        npc->vec.y = (level->height * level->map->tileHeight) - npc->height;
    return 1;
}


NPC* setNPC(int x, int y, int width, int height, Direction direction) {
    NPC* n = malloc(sizeof(NPC));
	if (n == NULL) {
        fprintf(stderr, "Malloc error while creating NPC!!!\n");
        return NULL;
    }
    
    n->name = "NPC...";

    n->vec = setVector(x, y);
    n->moveVec = setVector(0, 0);

    n->width = width;
	n->height = height;
    n->direction = direction;
    n->takingAction = 0;
    n->takingActionCounter = 0;
    n->maxTakingActionCounter = 0;
    SDL_Rect c = { n->vec.x, n->vec.y, n->width, n->height };
    n->col = c;
    return n;
}


Ground* setGround(float x, float y, short int width, short int height) {
    Ground* g = malloc(sizeof(Ground));
	if (g == NULL) {
        fprintf(stderr, "Malloc error while creating Ground!!!\n");
        return NULL;
    }
    g->vec = setVector(x, y);
    g->width = width;
    g->height = height;
    g->gid = 0;
    return g;
}


void drawNPCCollisions(NPC* npc, SDL_Renderer* renderer) {
    SDL_RenderDrawRect(renderer, &npc->col);
}



// ------------------ "PRIVATE" FUNCTIONS ------------------



int random(int min, int max) {
    int tmp;
    if (max>=min)
        max-= min;
    else
    {
        tmp= min - max;
        min= max;
        max= tmp;
    }
    return max ? (rand() % max + min) : min;
}
