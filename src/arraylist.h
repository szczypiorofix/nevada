#ifndef ARRAYLIST_H_
#define ARRAYLIST_H_
#pragma once


typedef struct ArrayList_Int {
    unsigned int size;
    unsigned int maxSize;
    unsigned int chunkSize;
    int* data;
} ArrayList_Int;




// ------------------ PUBLIC FUNCTIONS ------------------

ArrayList_Int* createArrayList(unsigned int initialSize, unsigned int cs);
int addIntToArrayList(ArrayList_Int* list, int v);
void printValues(ArrayList_Int* list);
int clearArrayList(ArrayList_Int** list);
int getFromArrayList(ArrayList_Int* list, unsigned int index);


#endif
