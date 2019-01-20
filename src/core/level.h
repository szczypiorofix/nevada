#ifndef LEVEL_H_
#define LEVEL_H_




typedef struct Level
{
    int* content;
    unsigned short width;
    unsigned short height;
} Level;



Level* getLevel(short n);


#endif
