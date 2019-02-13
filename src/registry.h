#ifndef REGISTRY_H_
#define REGISTRY_H_
#pragma once

typedef struct String {
    int length;
    char* str;
} String;

typedef struct Registry {
    unsigned int size;
    unsigned int maxSize;
    unsigned int chunkSize;
    char* tmp;
    String* strings;
} Registry;


int registryInit(void);
int registryRelease(void);
String createString(char* s);

#endif
