#ifndef FILEHANDLER_H_
#define FILEHANDLER_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum {
	true,
	false
} Boolean;


typedef struct {
	int size;
	char* content;
	char* name;
	Boolean success;
} BinaryFile;


char* copyStringFromPointer(const char* s);

int fromBinary(const char *s);


#endif
