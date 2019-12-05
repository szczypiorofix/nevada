#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "defines.h"


// ----------------------- CONSTANTS -----------------------
const int SCREEN_WIDTH		= 800;
const int SCREEN_HEIGHT		= 600;
const char* DIR_RES_IMAGES = "res/images/";
const short WALK_UP = 0;
const short WALK_RIGHT = 1;
const short WALK_DOWN = 2;
const short WALK_LEFT = 3;
const short NPC_SPEED = 1;



// ------------------ FORWARD DECLARATION ------------------
int fromBinary(const char *s);
char* copyStringFromPointer(const char* s);
int releaseString(void* string);


// Vector2
Vector2 setVector(float x, float y);
void rotateVector(Vector2* vector, float angle);
void addVector(Vector2* v1, const Vector2* v2);
void subVector(Vector2* v1, const Vector2* v2);
void mulVector(Vector2* v1, const Vector2* v2);
void divVector(Vector2* v1, const Vector2* v2);
void normalizeVetor(Vector2* v);


// ------------------ VECTOR2 FUNCTIONS ------------------

Vector2 setVector(float x, float y) {
	Vector2 v = {x, y};
	return v;
}


void addVector(Vector2* v1, const Vector2* v2) {
	v1->x += v2->x;
	v1->y += v2->y;
}


void subVector(Vector2* v1, const Vector2* v2) {
	v1->x -= v2->x;
	v1->y -= v2->y;
}

void mulVector(Vector2* v1, const Vector2* v2) {
	v1->x *= v2->x;
	v1->y *= v2->y;
}


void divVector(Vector2* v1, const Vector2* v2) {
	v1->x /= v2->x;
	v1->y /= v2->y;
}


void normalizeVetor(Vector2* v) {
	float mag = (float) sqrt(v->x * v->x + v->y * v->y);
	v->x = (float) (v->x / mag);
	v->y = (float) (v->y / mag);
}


// ------------------ COMMON FUNCTIONS ------------------

int fromBinary(const char *s) {
  return (int) strtol(s, NULL, 2);
}


char* copyStringFromPointer(const char* s) {
	int sl = strlen(s) + 1;
	char *stringcopy = malloc(sl);
	if (stringcopy) strcpy(stringcopy, s);
	else fprintf(stderr, " malloc failure!");
	return stringcopy;
}
