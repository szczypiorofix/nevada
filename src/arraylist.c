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



// ------------------ FUNCTIONS ------------------

ArrayList_Int* createArrayList(unsigned int initialSize, unsigned int chunkSize, unsigned int sizeOfType, unsigned int flags) {
    ArrayList_Int* list = malloc(sizeof(ArrayList_Int));
    if (list == NULL) {
        fprintf(stderr, "Cannot allocate memory for a new ArrayList_Int !\n");
        return NULL;
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
    printf("Adding value %i to ArrayList...\n", value);
    if (list->size < list->maxSize) {
        list->data[list->size] = value;
        list->size++;
    } else {
        fprintf(stderr, "Not enough memory. Calling realloc ...\n");
        int nextSize = ( list->sizeOfType * list->maxSize ) + ( list->sizeOfType * list->chunkSize );
        int* temp = realloc(list->data, nextSize);
        if (temp == NULL) {
            fprintf(stderr, "Cannot re-allocate memory for next chunk (%i) of elements in ArrayList !\n", list->chunkSize);
            return 0;
        } else {
            list->data = temp;
            list->maxSize += list->chunkSize;
            list->data[list->size] = value;
            list->size++;
        }
        
    }
    return 1;
}


int getFromArrayList(ArrayList_Int* list, unsigned int index) {
    if (index > list->size - 1) {
        fprintf(stderr, "ArrayList ERROR !!! Index %i out of array size (%i)!\n", index, list->size - 1);
        return 0;
    }
    return list->data[index];
}


int popArrayList(ArrayList_Int* list) {
    int p = 0;
    if (list->size > 0) {
        p = list->data[list->size - 1];
        
        // Change the last element to 0
        list->data[list->size - 1] = 0;

        if (list->size == list->maxSize - list->chunkSize + 1 && list->flags == ARRAYLIST_SHIRK_AFTER_DELETE) {
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
    int p = 0;
    if (list->size > 0) {
        p = list->data[0];
        list->size--;
        for (unsigned int i = 0; i < list->size; i++) {
            list->data[i] = list->data[i+1];
        }
    }
    return p;
}

int clearArrayList(ArrayList_Int** list) {
    free( (*list)->data );
    (*list)->data = NULL;
    free(*list);
    *list = NULL;
    return 1;
}


int shrinkArrayList(ArrayList_Int* list) {
    printf("Shrinking ArrayList maxSize: %i to size: %i.\n", (list->sizeOfType * list->maxSize), (list->sizeOfType * list->size));
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
    printf("ArrayList: size: %i, maxSize: %i, chunkSize: %i, sizeOfType: %i, memory reserved: %i, memory address: %p \n", list->size, list->maxSize, list->chunkSize, list->sizeOfType, list->sizeOfType * list->maxSize, (void *)list->data);
    printf("Data: ");
    if (list->size == 0) printf("<NULL>");
    for (unsigned int i = 0; i < list->size; i++) printf("%i,", list->data[i]);
    printf("\n");
}


int changeFlags(ArrayList_Int* list, unsigned int flags) {
    list->flags = flags;
    return 1;
}