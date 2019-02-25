#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "objects.h"



// ------------------ FORWARD DECLARATION ------------------
Player* resetPlayer(char* name, float x, float y, short int width, short int height);
void updateCamera(Camera* c, Player* player);
int getTileX(Player* p, unsigned int tw);
int getTileY(Player* p, unsigned int th);
int updateNPC(NPC* npc);
int random(int min, int max);
NPC* setNPC(int x, int y, int width, int height, Direction direction);
void updateCollisionsNPC(NPC* npc, Camera* cam);
Ground* setGround(float x, float y, short int width, short int height);
void updateCollisionsPlayer(Player* p, Camera* cam);


// ------------------ PUBLIC FUNCTIONS ------------------
Player* resetPlayer(char* name, float x, float y, short int width, short int height) {
    Player* p = malloc(sizeof(Player));
	if (p == NULL) {
        fprintf(stderr, "Malloc error while creating Player!!!\n");
        return NULL;
    }
    
    p->name = name;
    p->velX = 0.0f;
    p->velY = 0.0f;
    p->width = width;
	p->height = height;
    p->direction = DIR_RIGHT;
    
    p->vec = setVector(x, y);
    
    p->angle = 0;
    p->angleVel = 0;
    p->speed = 0;
    p->isMoving = 0;
    SDL_Rect cu = { p->vec.x, p->vec.y, p->width, 5 };
    p->col_up = cu;
    SDL_Rect cr = { p->vec.x + p->width, p->vec.y, 5, p->height };
    p->col_right = cr;
    SDL_Rect cd = { p->vec.x, p->vec.y + p->height, p->width, 5};
    p->col_down = cd;
    SDL_Rect cl = { p->vec.x, p->vec.y, 5, p->height };
    p->col_left = cl;
    return p;
}


void updateCamera(Camera* c, Player* player) {
    // c->x = - player.x + (SCREEN_WIDTH / 2) - (player.width / 2);
    // c->y = - player.y + (SCREEN_HEIGHT / 2) - (player.height / 2);
    // c->x = - player->x + (SCREEN_WIDTH / 2) - (player->width / 2);
    // c->y = - player->y + (SCREEN_HEIGHT / 2) - (player->height / 2);

    // c->offsetX = player->x + (player->width / 2) - (SCREEN_WIDTH / 2);
    // c->offsetY = player->y + (player->height / 2) - (SCREEN_HEIGHT / 2);

    // c->x = player->x + (player->width * scale) - (SCREEN_WIDTH / 2);
    // c->y = player->y + (player->height * scale) - (SCREEN_HEIGHT / 2);

    // c->x = player->x + (player->width)  - (SCREEN_WIDTH / 2);
    // c->y = player->y + (player->height) - (SCREEN_HEIGHT / 2);

    c->vec.x = player->vec.x + (player->width / 2) - (SCREEN_WIDTH / 2);
    c->vec.y = player->vec.y + (player->height / 2) - (SCREEN_HEIGHT / 2);

}


int getTileX(Player* p, unsigned int tw) {
    return ( (p->vec.x + (p->width / 2)) / tw );
}


int getTileY(Player* p, unsigned int th) {
    return ( (p->vec.y + (p->height / 2)) / th );
}


void updateCollisionsNPC(NPC* npc, Camera* cam) {
    SDL_Rect cu = { npc->x  + cam->vec.x, npc->y + cam->vec.y, npc->width, 5 };
    npc->col_up = cu;
    SDL_Rect cr = { npc->x + npc->width + cam->vec.x, npc->y + cam->vec.y, 5, npc->height };
    npc->col_right = cr;
    SDL_Rect cd = { npc->x + cam->vec.x, npc->y + npc->height + cam->vec.y, npc->width, 5};
    npc->col_down = cd;
    SDL_Rect cl = { npc->x + cam->vec.x, npc->y + cam->vec.y, 5, npc->height };
    npc->col_left = cl;
}


void updateCollisionsPlayer(Player* p, Camera* cam) {
    SDL_Rect cu = { p->vec.x  + cam->vec.x, p->vec.y + cam->vec.y, p->width, 5 };
    p->col_up = cu;
    SDL_Rect cr = { p->vec.x + p->width + cam->vec.x, p->vec.y + cam->vec.y, 5, p->height };
    p->col_right = cr;
    SDL_Rect cd = { p->vec.x + cam->vec.x, p->vec.y + p->height + cam->vec.y, p->width, 5};
    p->col_down = cd;
    SDL_Rect cl = { p->vec.x + cam->vec.x, p->vec.y + cam->vec.y, 5, p->height };
    p->col_left = cl;
}


int randomNPCActivity_move() {
    return random(60, 160);
}


int randomNPCActivity_wait() {
    return random(20, 150);
}


int updateNPC(NPC* npc) {
    
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
                npc->velX = 0;
                npc->velY = -NPC_SPEED;
                npc->direction = DIR_UP;
                npc->maxTakingActionCounter = randomNPCActivity_move();
                break;
            case DIR_RIGHT:
                npc->velX = NPC_SPEED;
                npc->velY = 0;
                npc->direction = DIR_RIGHT;
                npc->maxTakingActionCounter = randomNPCActivity_move();
                break;
            case DIR_DOWN:
                npc->velX = 0;
                npc->velY = NPC_SPEED;
                npc->direction = DIR_DOWN;
                npc->maxTakingActionCounter = randomNPCActivity_move();
                break;
            case DIR_LEFT:
                npc->velX = -NPC_SPEED;
                npc->velY = 0;
                npc->direction = DIR_LEFT;
                npc->maxTakingActionCounter = randomNPCActivity_move();
                break;
            // Additional directions
            case DIR_UP_RIGHT:
                npc->velX = NPC_SPEED;
                npc->velY = -NPC_SPEED;
                npc->direction = DIR_RIGHT;
                npc->maxTakingActionCounter = randomNPCActivity_move();
                break;
            case DIR_DOWN_RIGHT:
                npc->velX = NPC_SPEED;
                npc->velY = NPC_SPEED;
                npc->direction = DIR_RIGHT;
                npc->maxTakingActionCounter = randomNPCActivity_move();
                break;
            case DIR_DOWN_LEFT:
                npc->velX = -NPC_SPEED;
                npc->velY = NPC_SPEED;
                npc->direction = DIR_LEFT;
                npc->maxTakingActionCounter = randomNPCActivity_move();
                break;
            case DIR_UP_LEFT:
                npc->velX = -NPC_SPEED;
                npc->velY = -NPC_SPEED;
                npc->direction = DIR_LEFT;
                npc->maxTakingActionCounter = randomNPCActivity_move();
                break;
            }
            npc->takingAction = 1;
        } else {
            npc->takingAction = 0;
            npc->takingActionCounter = 0;
            npc->maxTakingActionCounter = randomNPCActivity_wait();
            npc->velX = 0;
            npc->velY = 0;
        }  
    }

    npc->x += npc->velX;
    npc->y += npc->velY;

    return 1;
}


NPC* setNPC(int x, int y, int width, int height, Direction direction) {
    NPC* n = malloc(sizeof(NPC));
	if (n == NULL) {
        fprintf(stderr, "Malloc error while creating NPC!!!\n");
        return NULL;
    }
    
    n->name = "NPC...";

    n->x = x;
    n->y = y;
    n->velX = 0;
    n->velY = 0;
    n->width = width;
	n->height = height;
    n->direction = direction;
    n->takingAction = 0;
    n->takingActionCounter = 0;
    n->maxTakingActionCounter = 0;
    SDL_Rect cu = { n->x, n->y, n->width, 5 };
    n->col_up = cu;
    SDL_Rect cr = { n->x + n->width, n->y, 5, n->height };
    n->col_right = cr;
    SDL_Rect cd = { n->x, n->y + n->height, n->width, 5};
    n->col_down = cd;
    SDL_Rect cl = { n->x, n->y, 5, n->height };
    n->col_left = cl;
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
