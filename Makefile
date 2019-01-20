CC=gcc
CFLAGS=-c
CWARNFLAGS=-Wall -Werror
ODIR=obj
LIBRARYPATH=-LC:\\mingw_dev_lib\\lib
INCLUDEPATH=-IC:\\mingw_dev_lib\\include
LINKERFLAGS=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

all: main.exe

main.exe: main.o game.o textures.o font.o level.o music.o
	$(CC) $(CWARNFLAGS) $(ODIR)/main.o $(ODIR)/game.o $(ODIR)/textures.o $(ODIR)/font.o $(ODIR)/level.o $(ODIR)/music.o $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o main.exe

main.o: src/main.c src/main.h src/core/game.h
	$(CC) $(CFLAGS) $(CWARNFLAGS) src/main.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/main.o

game.o: src/core/game.c src/core/game.h
	$(CC) $(CFLAGS) $(CWARNFLAGS) src/core/game.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/game.o

textures.o: src/graphics/textures.c src/graphics/textures.h
	$(CC) $(CFLAGS) $(CWARNFLAGS) src/graphics/textures.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/textures.o

font.o: src/graphics/font.c src/graphics/font.h
	$(CC) $(CFLAGS) $(CWARNFLAGS) src/graphics/font.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/font.o

level.o: src/core/level.c src/core/level.h
	$(CC) $(CFLAGS) $(CWARNFLAGS) src/core/level.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/level.o

music.o: src/sounds/music.c src/sounds/music.h
	$(CC) $(CFLAGS) $(CWARNFLAGS) src/sounds/music.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/music.o


# main.exe: main.o filehandler.o loghandler.o readbinary.o writebinary.o writestruct.o
# 	$(CC) obj/filehandler.o obj/loghandler.o obj/writebinary.o obj/readbinary.o obj/writestruct.o obj/main.o -o main.exe


# filehandler.o: src/core/filehandler.c src/core/filehandler.h
# 	$(CC) $(CFLAGS) src/core/filehandler.c -o obj/filehandler.o

# loghandler.o: src/core/loghandler.c src/core/loghandler.h
# 	$(CC) $(CFLAGS) src/core/loghandler.c -o obj/loghandler.o

# readbinary.o: src/modules/readbinary.c src/modules/readbinary.h src/core/filehandler.h
# 	$(CC) $(CFLAGS) src/modules/readbinary.c -o obj/readbinary.o

# writebinary.o: src/modules/writebinary.c src/modules/writebinary.h
# 	$(CC) $(CFLAGS) src/modules/writebinary.c -o obj/writebinary.o

# writestruct.o: src/modules/writestruct.c src/modules/writestruct.h
# 	$(CC) $(CFLAGS) src/modules/writestruct.c -o obj/writestruct.o

# .PHONY: clean

clean:
	del $(ODIR)\*.o main.exe