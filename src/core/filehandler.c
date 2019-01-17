#include "filehandler.h"

// FORWARD DECLARATION

char* copyStringFromPointer(const char* s);
int fromBinary(const char *s);


char* copyStringFromPointer(const char* s) {
	int sl = strlen(s) + 1;
	char *stringcopy = malloc(sl);
	if (stringcopy) strcpy(stringcopy, s);
	else fprintf(stderr, "malloc failure!");
	return stringcopy;
}



int fromBinary(const char *s) {
  return (int) strtol(s, NULL, 2);
}
