#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"


// ------------------ FORWARD DECLARATION ------------------
int fromBinary(const char *s);
char* getFromResourceImagesDir(char* file);
char* getFromResourceDir(char* file);
char* copyStringFromPointer(const char* s);



// ------------------ "PUBLIC" FUNCTIONS ------------------

int fromBinary(const char *s) {
  return (int) strtol(s, NULL, 2);
}

char* getFromResourceImagesDir(char* file) {
    char strTo[50] = DIR_RES_IMAGES;
    strncat(strTo, file, strlen(file));
	int strSize = strlen(strTo);
	// printf("StrTO: %s, string length: %i\n", strTo, strSize);
	char* r = malloc(sizeof(char) * strSize);
	r = strTo;
	return r;
}

char* getFromResourceDir(char* file) {
    char strTo[50] = DIR_RES;
    strncat(strTo, file, strlen(file));
	int strSize = strlen(strTo);
	// printf("StrTO: %s, string length: %i\n", strTo, strSize);
	char* r = malloc(sizeof(char) * strSize);
	r = strTo;
	return r;
}

char* copyStringFromPointer(const char* s) {
	int sl = strlen(s) + 1;
	char *stringcopy = malloc(sl);
	if (stringcopy) strcpy(stringcopy, s);
	else fprintf(stderr, " malloc failure!");
	return stringcopy;
}
