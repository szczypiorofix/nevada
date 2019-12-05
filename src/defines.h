#ifndef _DEFINES_H_
#define _DEFINES_H_
#pragma once


// ------------------ CONSTANTS ------------------

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const char* DIR_RES_IMAGES;
extern const short WALK_UP;
extern const short WALK_RIGHT;
extern const short WALK_DOWN;
extern const short WALK_LEFT;
extern const short NPC_SPEED;



// ------------------ VECTOR2 ------------------
typedef struct Vector2 {
    float x;
    float y;
} Vector2;


Vector2 setVector(float x, float y);
void rotateVector(Vector2* vector, float angle);
void addVector(Vector2* v1, const Vector2* v2);
void subVector(Vector2* v1, const Vector2* v2);
void mulVector(Vector2* v1, const Vector2* v2);
void divVector(Vector2* v1, const Vector2* v2);
void normalizeVetor(Vector2* v);


// ------------------ COMMON FUNCTIONS ------------------

int fromBinary(const char *s);
char* copyStringFromPointer(const char* s);


#endif
