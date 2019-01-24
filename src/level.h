#pragma once


typedef struct Level {
    int* content;
    unsigned short width;
    unsigned short height;
    unsigned int size;
} Level;



void parsing(const char* fileName);
