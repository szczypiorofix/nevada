#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL_image.h>
#include "textures.h"
#include "level.h"



// ------------------ FORWARD DECLARATION ------------------
SpriteSheet* loadSpriteSheet(char* fileName, SDL_Renderer* renderer, unsigned int spriteWidth, unsigned int spriteHeigth);
void freeTexture(SpriteSheet* t);
void renderTexture(SpriteSheet* t, SDL_Renderer* renderer, SDL_Rect* clip, int x, int y, int scale, double angle, SDL_Point* center, SDL_RendererFlip flip);
int releaseAnimation(Animation** an);

int checkCollision(SDL_Rect r1, SDL_Rect r2);

// ------------------ "PUBLIC" FUNCTIONS ------------------


int releaseAnimation(Animation** an) {
    // for (int i = 0; i < (*an)->size; i++) {
    //     free( (*an)->frames[i] );
    // }
    free( (*an)->frames );
    (*an)->frames = NULL;
    return 1;
}


void freeTexture(SpriteSheet* t) {
    if (t->mTexture != NULL) {
        SDL_DestroyTexture(t->mTexture);
        t->mTexture = NULL;
        free(t);
    }
}


void renderTexture(SpriteSheet* t, SDL_Renderer* renderer, SDL_Rect* clip, int x, int y, int scale, double angle, SDL_Point* center, SDL_RendererFlip flip) { 
    SDL_Rect renderQuad = {x, y, t->tileWidth * scale, t->tileHeight * scale};
    // if (clip != NULL) {
    //     renderQuad.w = clip->w;
	// 	renderQuad.h = clip->h;
    // }
    SDL_RenderCopyEx(renderer, t->mTexture, clip, &renderQuad, angle, center, flip);
}


SpriteSheet* loadSpriteSheet(char* fileName, SDL_Renderer* renderer, unsigned int tileWidth, unsigned int tileHeight) {
    SpriteSheet* t = malloc(sizeof(SpriteSheet));
    if (t == NULL) {
        fprintf(stderr, "Cannot load sprite sheet: %s !\n", fileName);
        return NULL;
    }
    SDL_Texture* newTexture = NULL;
    t->name = fileName;
    char str[50] = DIR_RES_IMAGES;
    const char *strFrom = fileName;
    strcat (str, strFrom);

    SDL_Surface* loadedSurface = IMG_Load(str);
    if (loadedSurface == NULL) {
        printf( "Unable to load image %s! SDL_image Error: %s\n", str, IMG_GetError());
        t = NULL;
    } else {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", str, SDL_GetError());
        } else {
            t->width = loadedSurface->w;
            t->height = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    t->mTexture = newTexture;
    t->tileWidth = tileWidth;
    t->tileHeight = tileHeight;
    return t;
}


SDL_Rect* createRectsForSprites(Level* level, int layerCount, const unsigned int size, SpriteSheet* t) {
    SDL_Rect* l = malloc(sizeof(SDL_Rect) * size);
    int col = t->width / t->tileWidth;

    for (unsigned int i = 0; i < level->size; i++) {

        SDL_Rect r = {
            r.x = ((level->content[layerCount].data[i] - 1) % col) * t->tileWidth,
            r.y = ((level->content[layerCount].data[i] - 1) / col) * t->tileHeight,
            r.w = t->tileWidth,
            r.h = t->tileHeight
        };

        if ( (level->content[layerCount].data[i] - 1) == 0 ) {
            r.x = -1;
            r.y = -1;
            r.w = -1;
            r.h = -1;
        }
        l[i] = r;

    }

    return l;
}

Animation* prepareAnimation(SpriteSheet* t, unsigned int speed, unsigned int sw, unsigned int sh, const unsigned int size, unsigned int* frames) {
    Animation* anim = malloc(sizeof(Animation));
    if (anim == NULL) return NULL;

    anim->size = size;
    anim->speed = speed;
    anim->counter = 0;
    anim->curFrame = 0;
    anim->spriteSheet = t;

    anim->frames = malloc(sizeof(SDL_Rect) * size);
    if (anim->frames == NULL) return NULL;
    for (unsigned int i = 0; i < size; i++) {
        int col = t->width / t->tileWidth;
        SDL_Rect r = {
            r.x = ((frames[i] - 1) % col) * sw,
            r.y = ((frames[i] - 1) / col) * sh,
            r.w = sw,
            r.h = sh
        };
        anim->frames[i] = r;
    }
    return anim;
}

int nextFrame(Animation* an) {
    an->counter++;
    if (an->counter > an->speed) {
        an->counter = 0;
        an->curFrame++;
        if (an->curFrame >= an->size) {
            an->curFrame = 0;
        }
    }
    return an->curFrame;
}


int checkCollision(SDL_Rect r1, SDL_Rect r2) {
    return (
        r1.x + r1.w > r2.x &&
        r1.y + r1.h > r2.y &&
        r1.x < r2.x + r2.w &&
        r1.y < r2.y + r2.h
    );
}

// ------------------ "PRIVATE" FUNCTIONS ------------------
