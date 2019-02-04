#include <stdio.h>
#include <stdlib.h>

#include "arraylist.h"



// ------------------ FORWARD DECLARATION ------------------

ArrayList_Int* createArrayList(unsigned int initialSize, unsigned int cs);
int addIntToArrayList(ArrayList_Int* list, int v);
void printValues(ArrayList_Int* list);
int clearArrayList(ArrayList_Int** list);
int getFromArrayList(ArrayList_Int* list, unsigned int index);



ArrayList_Int* createArrayList(unsigned int initialSize, unsigned int cs) {
    ArrayList_Int* list = malloc(sizeof(ArrayList_Int));
    if (list == NULL) {
        fprintf(stderr, "Cannot alocate memory for a new ArrayList_Int !\n");
        return NULL;
    }
    list->size = 0;
    list->chunkSize = cs;
    list->maxSize = initialSize;

    printf("Allocating %i bytes (%i ints) of memory for data.\n", sizeof(int) * initialSize, initialSize);
    list->data = malloc(sizeof(int) * initialSize);
    if (list->data == NULL) {
        fprintf(stderr, "Cannot alocate memory for %i (int) elements in ArrayList !\n", initialSize);
        return NULL;
    }
    return list;
}

void printValues(ArrayList_Int* list) {
    printf("\nArrayList size: %i, maxSize: %i, chunkSize: %i\n", list->size, list->maxSize, list->chunkSize);
    for (unsigned int i = 0; i < list->size; i++) {
        printf("Element %i - value: %i \n", i, list->data[i]);
    }
}

int addIntToArrayList(ArrayList_Int* list, int v) {
    printf("Adding value %i to ArrayList...\n", v);
    if (list->size < list->maxSize) {
        list->data[list->size] = v;
        list->size++;
    } else {
        fprintf(stderr, "Not enough memory. Calling realloc ...\n");
        int nextSize = ( sizeof(int) * list->maxSize ) + ( sizeof(int) * list->chunkSize );
        int* temp = realloc(list->data, nextSize);
        printf("Re-allocating memory for next (%i) chunks of elements in ArrayList...\n", list->chunkSize);
        if (temp == NULL) {
            fprintf(stderr, "Cannot re-alocate memory for next chunk (%i) of elements in ArrayList !\n", list->chunkSize);
            return 0;
        } else {
            list->data = temp;
            list->maxSize += list->chunkSize;
            list->data[list->size] = v;
            list->size++;
            printf("Size of allocated memory: %i bytes for next %i ints. Total elements: %i.\n", nextSize, list->chunkSize, nextSize / (sizeof(int)));
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

int clearArrayList(ArrayList_Int** list) {
    free(*list);
    *list = NULL;
    return 1;
}
