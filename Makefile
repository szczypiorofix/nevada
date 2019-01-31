## My makefile
CC=gcc
CFLAGS=-c
CWARNFLAGS=-Wall -Werror
ODIR=obj
SOURCE=src
LIBRARYPATH=-LC:\\mingw_dev_lib\\lib
INCLUDEPATH=-IC:\\mingw_dev_lib\\include
LINKERFLAGS=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -llibxml2 -liconv -llua -g -std=c11
## Optimize, -mwindows -no console output
OPTIMIZEFLAG=-Os -mwindows

all: bin/main.exe

bin/main.exe: $(ODIR)/main.o
	$(CC) $(CWARNFLAGS) $(ODIR)/main.o $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o bin/main.exe

$(ODIR)/main.o: $(SOURCE)/main.c $(SOURCE)/main.h
	$(CC) $(CFLAGS) $(CWARNFLAGS) $(SOURCE)/main.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/main.o


clean:
	del $(ODIR)\*.o bin\main.exe
