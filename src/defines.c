#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"


// ------------------ FORWARD DECLARATION ------------------
int fromBinary(const char *s);
char* getFromResourceImagesDir(char* file);
char* getFromResourceDir(char* file);
char* copyStringFromPointer(const char* s);
int releaseString(void* string);




// ------------------ COMMON FUNCTIONS ------------------

int fromBinary(const char *s) {
  return (int) strtol(s, NULL, 2);
}

char* getFromResourceImagesDir(char* file) {
	const char *dir = DIR_RES_IMAGES;
	char *result = malloc(strlen(file) + strlen(dir) + 1);
	strcpy(result, dir);
	strncat(result, file, strlen(file));
	return result;
}

char* getFromResourceDir(char* file) {
	const char *dir = DIR_RES;
	char *result = malloc(strlen(file) + strlen(dir) + 1);
	strcpy(result, dir);
	strncat(result, file, strlen(file));
	return result;
}

char* copyStringFromPointer(const char* s) {
	int sl = strlen(s) + 1;
	char *stringcopy = malloc(sl);
	if (stringcopy) strcpy(stringcopy, s);
	else fprintf(stderr, " malloc failure!");
	return stringcopy;
}


