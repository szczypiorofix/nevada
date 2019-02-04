#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "textures.h"
#include "engine.h"
#include "level.h"



// ------------------ FORWARD DECLARATION ------------------
Texture* loadSpriteSheet(char* fileName, enum SpriteSheets spritesheet, SDL_Renderer* renderer, unsigned int spriteWidth, unsigned int spriteHeigth);



// ------------------ "PUBLIC" FUNCTIONS ------------------

void freeTexture(Texture* t) {
    if (t->mTexture != NULL) {
        SDL_DestroyTexture(t->mTexture);
        t->mTexture = NULL;
        free(t);
    }
}


void renderTexture(Texture* t, struct Engine* engine, SDL_Rect* clip, int x, int y, unsigned int width, unsigned int height) {
    SDL_Rect renderQuad = {x, y, width, height};
    SDL_RenderCopy(engine->renderer, t->mTexture, clip, &renderQuad);
}


Texture* loadSpriteSheet(char* fileName, enum SpriteSheets spritesheet, SDL_Renderer* renderer, unsigned int spriteWidth, unsigned int spriteHeigth) {
    Texture* t = malloc(sizeof(Texture));
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
    t->sWidth = spriteWidth;
    t->sHeight = spriteHeigth;
    t->spriteSheet = spritesheet;
    return t;
}


SDL_Rect* getSpriteI(Texture* t, int index, unsigned int width, unsigned int height) {
    SDL_Rect* r = malloc(sizeof(SDL_Rect));
    int col = t->width / width;
    r->x = ((index - 1) % col) * width;
    r->y = ((index - 1) / col) * height;
    r->w = width;
    r->h = height;
    return r;
}

SDL_Rect* createRectsForSprites(Level* level, int layerCount, const unsigned int size, Texture* t) {
    SDL_Rect* l = malloc(sizeof(SDL_Rect) * size);
    for (unsigned int i = 0; i < level->size; i++)
        l[i] = *getSpriteI(t, level->content[layerCount].data[i], t->sWidth, t->sHeight);
    return l;
}

Animation* prepareAnimation(Texture* t, unsigned int speed, unsigned int sw, unsigned int sh, const unsigned int size, unsigned int* frames) {
    Animation* anim = malloc(sizeof(Animation));
    if (anim == NULL) return NULL;

    anim->size = size;
    anim->speed = speed;
    anim->counter = 0;
    anim->curFrame = 0;

    anim->frames = malloc(sizeof(SDL_Rect) * size);
    if (anim->frames == NULL) return NULL;
    for (unsigned int i = 0; i < size; i++) {
        anim->frames[i] = *getSpriteI(t, frames[i], sw, sh);
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


// ------------------ "PRIVATE" FUNCTIONS ------------------