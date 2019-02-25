#ifndef DEFINES_H_
#define DEFINES_H_
#pragma once


#include "registry.h"

// ------------------ CONSTANTS ------------------

#define WALK_UP 0
#define WALK_RIGHT 1
#define WALK_DOWN 2
#define WALK_LEFT 3


#define SPEED 2
#define NPC_SPEED 1


// Booleans
#define FALSE 0
#define TRUE  1
typedef unsigned char bool;


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define DIR_RES_IMAGES "res/images/"
#define DIR_RES "res/"


#define PI 3.14159265
#define DEG_TO_RAD PI / 180.0f


// ------------------ TYPEDEFS ------------------

// BYTE
typedef char byte;
typedef unsigned char ubyte;

// INT16
typedef signed short int int16;
typedef unsigned short int uint16;

// INT32
typedef signed int int32;
typedef unsigned int uint32;

// INT64
typedef signed long int int64;
typedef unsigned long int uint64;



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


// ------------------ ENUMS ------------------





// ------------------ COMMON FUNCTIONS ------------------

int fromBinary(const char *s);
char* copyStringFromPointer(const char* s);


#endif
