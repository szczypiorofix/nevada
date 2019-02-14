#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "objects.h"



// ------------------ FORWARD DECLARATION ------------------
Player* resetPlayer(void);
void updateCamera(Camera* c, Player player);
int getTileX(Player* p, unsigned int tw);
int getTileY(Player* p, unsigned int th);
int updateNPC(NPC* npc);
int velNPC(NPC* npc);
int random(int min, int max);


// ------------------ PUBLIC FUNCTIONS ------------------
Player* resetPlayer(void) {
    Player* p = malloc(sizeof(Player));
	if (p == NULL) return NULL;
    p->x = SCREEN_WIDTH / 2;
    p->y = SCREEN_HEIGHT / 2;
    p->velX = 0;
    p->velY = 0;
    p->width = 52;
	p->height = 72;
    p->direction = DIR_RIGHT;
    p->isMoving = 0;
    return p;
}

void updateCamera(Camera* c, Player player) {
    c->x = - player.x + (SCREEN_WIDTH / 2) - (player.width / 2);
    c->y = - player.y + (SCREEN_HEIGHT / 2) - (player.height / 2);
    c->offsetX = - player.x + (SCREEN_WIDTH / 2) - (player.width / 2);
    c->offsetY = - player.y + (SCREEN_HEIGHT / 2) - (player.height / 2);
}

int getTileX(Player* p, unsigned int tw) {
    return ( (p->x + (p->width / 2)) / tw );
}

int getTileY(Player* p, unsigned int th) {
    return ( (p->y + (p->height / 2)) / th );
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



// ------------------ "RIVATE" FUNCTIONS ------------------



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
