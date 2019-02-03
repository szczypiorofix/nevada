#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "objects.h"




Player* resetPlayer(void);
void updateCamera(Camera* c, Player player);
int getTileX(Player* p, unsigned int tw);
int getTileY(Player* p, unsigned int th);




Player* resetPlayer(void) {
    Player* p = malloc(sizeof(Player));
	if (p == NULL) return NULL;
    p->x = SCREEN_WIDTH / 2;
    p->y = SCREEN_HEIGHT / 2;
    p->velX = 0;
    p->velY = 0;
    p->width = 52;
	p->height = 72;
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
