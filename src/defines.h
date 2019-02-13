#ifndef DEFINES_H_
#define DEFINES_H_
#pragma once


// ------------------ CONSTANTS ------------------

#define WALK_UP 0
#define WALK_RIGHT 1
#define WALK_DOWN 2
#define WALK_LEFT 3

#define SPEED 2

// Booleans
#define FALSE 0
#define TRUE  1
typedef unsigned char bool;


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define DIR_RES_IMAGES "res/images/"
#define DIR_RES "res/"


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




// ------------------ ENUMS ------------------

enum SpriteSheets {
    NPC_SPRITESHEET,                // 0
    GRASSLAND1_SPRITESHEET          // 1
} spriteSheets;





// ------------------ COMMON FUNCTIONS ------------------

int fromBinary(const char *s);
char* getFromResourceImagesDir(char* file);
char* getFromResourceDir(char* file);
char* copyStringFromPointer(const char* s);


#endif
