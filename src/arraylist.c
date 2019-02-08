#include <stdio.h>
#include <stdlib.h>

#include "arraylist.h"



// ------------------ FORWARD DECLARATION ------------------

ArrayList_Int* createArrayList(unsigned int initialSize, unsigned int chunkSize, unsigned int sizeOfType, unsigned int flags);
int addIntToArrayList(ArrayList_Int* list, int value);
void printValues(ArrayList_Int* list);
int clearArrayList(ArrayList_Int** list);
int getFromArrayList(ArrayList_Int* list, unsigned int index);
int popArrayList(ArrayList_Int* list);
int shiftArrayList(ArrayList_Int* list);
void dumpArrayList(ArrayList_Int* list);
int shrinkArrayList(ArrayList_Int* list);
int changeFlags(ArrayList_Int* list, unsigned int flags);
int removeFromArrayList(ArrayList_Int* list, unsigned int index);


// Different types of data

ArrayList* createList(unsigned int initialSize, unsigned int chunkSize, unsigned int sizeOfType, unsigned int flags);
int addCharToList(ArrayList* list, char item);
int addIntToList(ArrayList* list, int item);
int clearList(ArrayList** list);
int getIntFromArray(ArrayList* list, unsigned int index);
char getCharFromArray(ArrayList* list, unsigned int index);



char getCharFromArray(ArrayList* list, unsigned int index) {
    if (list == NULL) {
        fprintf(stderr, "ERROR !!! ArrayList cannot be null !!!\n");
        return 0;
    }
    if (index > list->size - 1) {
        fprintf(stderr, "ArrayList ERROR !!! Index %i out of array size (%i)!\n", index, list->size - 1);
        return 0;
    }
    return list->data[index]->c;
}


int getIntFromArray(ArrayList* list, unsigned int index) {
    if (list == NULL) {
        fprintf(stderr, "ERROR !!! ArrayList cannot be null !!!\n");
        return 0;
    }
    if (index > list->size - 1) {
        fprintf(stderr, "ArrayList ERROR !!! Index %i out of array size (%i)!\n", index, list->size - 1);
        return 0;
    }
    return list->data[index]->i;
}


int clearList(ArrayList** list) {
    if (list == NULL) {
        fprintf(stderr, "ArrayList cannot be NULL !!!\n");
        return 0;
    }
    printf("Clearing list...\n");
    for (unsigned int i = 0; i < (*list)->maxSize; i++) {
        free( (*list)->data[i] );
        (*list)->data[i] = NULL;
    }
    free((*list)->data);
    (*list)->data = NULL;
    printf("Free list ... \n");
    free(*list);
    *list = NULL;
    return 1;
}

int addIntToList(ArrayList* list, int item) {
    if (list == NULL) {
        fprintf(stderr, "ArrayList cannot be NULL !!!\n");
        return 0;
    }
    printf("Adding value item %i to ArrayList...\n", item);
    if (list->size < list->maxSize) {
        list->data[list->size]->i = item;
        list->size++;
        return 1;
    }
    return 0;
}


int addCharToList(ArrayList* list, char item) {
    if (list == NULL) {
        fprintf(stderr, "ArrayList cannot be NULL !!!\n");
        return 0;
    }
    printf("Adding value item %c to ArrayList...\n", item);
    if (list->size < list->maxSize) {
        list->data[list->size]->c = item;
        list->size++;
    }
    return 1;
}


ArrayList* createList(unsigned int initialSize, unsigned int chunkSize, unsigned int sizeOfType, unsigned int flags) {
    ArrayList* list = malloc(sizeof(ArrayList));
    if (list == NULL) {
        fprintf(stderr, "Cannot allocate memory for a new ArrayList_Int !\n");
        return NULL;
    }

    list->size = 0;
    list->chunkSize = chunkSize;
    list->maxSize = initialSize;
    list->sizeOfType = sizeOfType;
    list->flags = flags;

    list->data = malloc(sizeof(void*) * initialSize);

    for (unsigned int i = 0; i < list->maxSize; i++) {
        list->data[i] = malloc(sizeOfType);
        if (list->data[i] == NULL) {
            fprintf(stderr, "Cannot allocate memory for %i (int) elements in ArrayList !\n", initialSize);
            return NULL;
        }
    }

    return list;
}




// ------------------ FUNCTIONS ------------------

ArrayList_Int* createArrayList(unsigned int initialSize, unsigned int chunkSize, unsigned int sizeOfType, unsigned int flags) {
    ArrayList_Int* list = malloc(sizeof(ArrayList_Int));
    if (list == NULL) {
        fprintf(stderr, "Cannot allocate memory for a new ArrayList_Int !\n");
        exit(0);
    }
    list->size = 0;
    list->chunkSize = chunkSize;
    list->maxSize = initialSize;
    list->sizeOfType = sizeOfType;
    list->flags = flags;

    list->data = malloc(sizeOfType * initialSize);
    if (list->data == NULL) {
        fprintf(stderr, "Cannot allocate memory for %i (int) elements in ArrayList !\n", initialSize);
        return NULL;
    }
    return list;
}


void printValues(ArrayList_Int* list) {
    if (list != NULL) {
        printf("\nArrayList size: %i, maxSize: %i, chunkSize: %i\n", list->size, list->maxSize, list->chunkSize);
        for (unsigned int i = 0; i < list->size; i++) {
            printf("Element %i - value: %i \n", i, list->data[i]);
        }
    } else {
        fprintf(stderr, "ArrayList is NULL !!!\n");
    }
}


int addIntToArrayList(ArrayList_Int* list, int value) {
    if (list == NULL) {
        fprintf(stderr, "ERROR !!! ArrayList cannot be null !!!\n");
        return 0;
    }
    printf("Adding value %i to ArrayList...\n", value);
    if (list->size < list->maxSize) {
        list->data[list->size] = value;
    } else {
        // fprintf(stderr, "Not enough memory. Calling realloc ...\n");
        int nextSize = ( list->sizeOfType * list->maxSize ) + ( list->sizeOfType * list->chunkSize );
        int* temp = realloc(list->data, nextSize);
        if (temp == NULL) {
            fprintf(stderr, "Cannot re-allocate memory for next chunk (%i) of elements in ArrayList !\n", list->chunkSize);
            return 0;
        } else {
            list->data = temp;
            list->maxSize += list->chunkSize;
            list->data[list->size] = value;
        }   
    }
    list->size++;
    return 1;
}


int getFromArrayList(ArrayList_Int* list, unsigned int index) {
    if (list == NULL) {
        fprintf(stderr, "ERROR !!! ArrayList cannot be null !!!\n");
        return 0;
    }
    if (index > list->size - 1) {
        fprintf(stderr, "ArrayList ERROR !!! Index %i out of array size (%i)!\n", index, list->size - 1);
        return 0;
    }
    return list->data[index];
}


int popArrayList(ArrayList_Int* list) {
    if (list == NULL) {
        fprintf(stderr, "ERROR !!! ArrayList cannot be null !!!\n");
        return 0;
    }
    int p = 0;
    if (list->size > 0) {
        p = list->data[list->size - 1];
        
        // Change the last element to 0
        list->data[list->size - 1] = 0;

        if (list->size == list->maxSize - list->chunkSize + 1 && list->flags == ARRAYLIST_SHRINK_AFTER_DELETE) {
            int* temp = NULL;
            temp = realloc(list->data, (list->sizeOfType * list->size) - list->sizeOfType );
            if (temp == NULL) {
                fprintf(stderr, "Error in re-allocating memory!\n");
                return 0;
            } else {
                list->data = temp;
                shrinkArrayList(list);
            }
        }
        list->size--;
    } else {
        fprintf(stderr, "ArrayList is empty !\n");
    }
    return p;
}


int shiftArrayList(ArrayList_Int* list) {
    if (list == NULL) {
        fprintf(stderr, "ERROR !!! ArrayList cannot be null !!!\n");
        return 0;
    }
    int p = 0;
    if (list->size > 0) {
        p = list->data[0];
        for (unsigned int i = 0; i < list->size - 1; i++) {
            list->data[i] = list->data[i + 1];
        }
        list->size--;
        shrinkArrayList(list);
    } else {
        fprintf(stderr, "ArrayList is empty !\n");
    }
    return p;
}

int removeFromArrayList(ArrayList_Int* list, unsigned int index) {
    if (list == NULL) {
        fprintf(stderr, "ERROR !!! ArrayList cannot be null !!!\n");
        return 0;
    }
    if (index > list->size - 1) {
        fprintf(stderr, "ArrayList ERROR !!! Index %i out of array size (%i)!\n", index, list->size - 1);
        return 0;
    }
    printf("Removed element %i at index %i\n", list->data[index], index);
    for (unsigned int i = 0; i < list->size - 1; i++) {
        if (i >= index) {
            list->data[i] = list->data[i + 1];
        }
    }
    list->size--;
    shrinkArrayList(list);
    return 1;
}

int clearArrayList(ArrayList_Int** list) {
    if (list == NULL) {
        fprintf(stderr, "ERROR !!! ArrayList cannot be null !!!\n");
        return 0;
    }
    free( (*list)->data );
    (*list)->data = NULL;
    free(*list);
    *list = NULL;
    return 1;
}


int shrinkArrayList(ArrayList_Int* list) {
    if (list == NULL) {
        fprintf(stderr, "ERROR !!! ArrayList cannot be null !!!\n");
        return 0;
    }
    // printf("Shrinking ArrayList maxSize: %i to size: %i.\n", (list->sizeOfType * list->maxSize), (list->sizeOfType * list->size));
    if (list->size > 0) {
        int* temp = realloc(list->data, (list->sizeOfType * list->size));
        if (temp == NULL) {
            fprintf(stderr, "Error in re-allocating memory !!!\n");
            return 0;
        } else {
            list->data = temp;
            list->maxSize = list->size;
        }
    }
    return 1;
}


void dumpArrayList(ArrayList_Int* list) {
    if (list == NULL) {
        fprintf(stderr, "ERROR !!! ArrayList cannot be null !!!\n");
        exit(0);
    }
    printf("ArrayList: size: %i, maxSize: %i, chunkSize: %i, sizeOfType: %i, memory reserved: %i, memory address: %p \n", list->size, list->maxSize, list->chunkSize, list->sizeOfType, list->sizeOfType * list->maxSize, (void *)list->data);
    printf("Data: ");
    if (list->size == 0) printf("<NULL>");
    for (unsigned int i = 0; i < list->size; i++) printf("%i,", list->data[i]);
    printf("\n");
}


int changeFlags(ArrayList_Int* list, unsigned int flags) {
    if (list == NULL) {
        fprintf(stderr, "ERROR !!! ArrayList cannot be null !!!\n");
        return 0;
    }
    list->flags = flags;
    return 1;
}