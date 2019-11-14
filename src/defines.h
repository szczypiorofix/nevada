#ifndef _DEFINES_H_
#define _DEFINES_H_
#pragma once


// ------------------ CONSTANTS ------------------

#define WALK_UP 0
#define WALK_RIGHT 1
#define WALK_DOWN 2
#define WALK_LEFT 3


#define NPC_SPEED 1


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define DIR_RES_IMAGES "res/images/"
#define DIR_RES "res/"



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
