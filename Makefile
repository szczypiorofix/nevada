## My makefile
CC=gcc
CFLAGS=-c
CWARNFLAGS=-Wall -Werror
ODIR=obj
LIBRARYPATH=-LC:\\mingw_dev_lib\\lib
INCLUDEPATH=-IC:\\mingw_dev_lib\\include
LINKERFLAGS=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -llibxml2 -liconv -llua -g
OPTIMIZEFLAG=-Os -std=c99

all: main.exe

main.exe: main.o game.o textures.o level.o
	$(CC) $(CWARNFLAGS) $(ODIR)/main.o $(ODIR)/game.o $(ODIR)/textures.o $(ODIR)/level.o $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o bin/main.exe

main.o: src/main.c src/main.h src/game.h
	$(CC) $(CFLAGS) $(CWARNFLAGS) src/main.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/main.o

game.o: src/game.c src/game.h
	$(CC) $(CFLAGS) $(CWARNFLAGS) src/game.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/game.o

textures.o: src/textures.c src/textures.h
	$(CC) $(CFLAGS) $(CWARNFLAGS) src/textures.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/textures.o

level.o: src/level.c src/level.h
	$(CC) $(CFLAGS) $(CWARNFLAGS) src/level.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/level.o


clean:
	del $(ODIR)\*.o main.exe
