CC=gcc
CFLAGS=-c
CWARNFLAGS=-Wall -Werror
ODIR=obj
LIBRARYPATH=-LC:\\mingw_dev_lib\\lib
INCLUDEPATH=-IC:\\mingw_dev_lib\\include
LINKERFLAGS=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lxml2

all: main.exe

main.exe: main.o game.o textures.o font.o
	$(CC) $(CWARNFLAGS) $(ODIR)/main.o $(ODIR)/game.o $(ODIR)/textures.o $(ODIR)/font.o $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o main.exe

main.o: src/main.c src/main.h src/game.h
	$(CC) $(CFLAGS) $(CWARNFLAGS) src/main.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/main.o

game.o: src/game.c src/game.h
	$(CC) $(CFLAGS) $(CWARNFLAGS) src/game.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/game.o

textures.o: src/textures.c src/textures.h
	$(CC) $(CFLAGS) $(CWARNFLAGS) src/textures.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/textures.o

font.o: src/font.c src/font.h
	$(CC) $(CFLAGS) $(CWARNFLAGS) src/font.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/font.o

clean:
	del $(ODIR)\*.o main.exe
