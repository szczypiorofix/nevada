#ifndef WRITESTRUCT_H_
#define WRITESTRUCT_H_

#include <stdio.h>
#include <stdlib.h>


typedef struct {
    int id; 
    char * fname; 
    char * lname; 
} Person;


int writeStruct();



#endif