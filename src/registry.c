#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registry.h"





// ------------------ FORWARD DECLARATION ------------------

int releaseString(void* string);

int registryInit(void);
int registryRelease(void);
String createString(char* s);



Registry* registry;




// ------------------ PRIVATE FUNCTIONS ------------------
int releaseString(void* string) {
	if (string == NULL) {
		  fprintf(stderr, "ERROR !!! String cannot be NULL !!!\n");
      return 0;
	}
	printf("Releasing string at %p\n", string);
	free(string);
	return 1;
}



// ------------------ PUBLIC FUNCTIONS ------------------
int registryInit(void) {
    registry = malloc(sizeof(Registry));
    if (registry == NULL) {
        fprintf(stderr, "Cannot allocate memory for a Registry !\n");
        return 0;
    }

    registry->tmp = NULL;

    registry->size = 0; // initial size = 0
    registry->maxSize = 10; // initial maxSize
    registry->chunkSize = 6; // initial chunkSize

    registry->strings = malloc(registry->maxSize * sizeof(String));
    if (registry->strings == NULL) {
        fprintf(stderr, "Cannot allocate memory for a registry strings !\n");
        return 0;
    }

    for (unsigned int i = 0; i < registry->maxSize; i++) {
        registry->strings[i].length = 0;
        registry->strings[i].str = NULL;
    }

    return 1;
}

int registryRelease(void) {
    if (registry == NULL) {
        fprintf(stderr, "AssetsList cannot be NULL !!!\n");
        return 0;
    }
    printf("Clearing registry data ...\n");
    free(registry->strings);
    registry->strings = NULL;

    printf("Clearing temp string ...\n");
    if (registry->tmp != NULL) free(registry->tmp);
    registry->tmp = NULL;

    printf("Clearing registry ...\n");
    free(registry);
    registry = NULL;
    return 1;
}

String createString(char* s) {
    int len = strlen(s);
    registry->strings[registry->size].str = s;
    registry->strings[registry->size].length = len;
    registry->size++;
    return registry->strings[registry->size - 1];
}
