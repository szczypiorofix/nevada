## My makefile
CC=gcc
CFLAGS=-c
CWARNFLAGS=-Wall -Wextra -Wpedantic #-Werror
ODIR=obj
SOURCE=src
LIBRARYPATH=-LC:\\mingw_dev_lib\\lib
INCLUDEPATH=-IC:\\mingw_dev_lib\\include
LINKERFLAGS=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -llibxml2 -liconv -llua -g -std=c11
## Optimize, -mwindows -no console output
OPTIMIZEFLAG=-Os -mwindows

all: bin/main.exe

# main file
bin/main.exe: $(ODIR)/main.o $(ODIR)/defines.o $(ODIR)/engine.o $(ODIR)/list.o $(ODIR)/level.o $(ODIR)/textures.o $(ODIR)/objects.o
	$(CC) $(CWARNFLAGS) $(ODIR)/main.o $(ODIR)/defines.o $(ODIR)/engine.o $(ODIR)/list.o $(ODIR)/level.o $(ODIR)/textures.o $(ODIR)/objects.o $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o bin/main.exe


# main.o
$(ODIR)/main.o: $(SOURCE)/main.c $(SOURCE)/main.h
	$(CC) $(CFLAGS) $(CWARNFLAGS) $(SOURCE)/main.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/main.o

# defines.o
$(ODIR)/defines.o: $(SOURCE)/defines.c $(SOURCE)/defines.h
	$(CC) $(CFLAGS) $(CWARNFLAGS) $(SOURCE)/defines.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/defines.o

# engine.o
$(ODIR)/engine.o: $(SOURCE)/engine.c $(SOURCE)/engine.h
	$(CC) $(CFLAGS) $(CWARNFLAGS) $(SOURCE)/engine.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/engine.o

# list.o
$(ODIR)/list.o: $(SOURCE)/list.c $(SOURCE)/list.h
	$(CC) $(CFLAGS) $(CWARNFLAGS) $(SOURCE)/list.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/list.o

# level.o
$(ODIR)/level.o: $(SOURCE)/level.c $(SOURCE)/level.h
	$(CC) $(CFLAGS) $(CWARNFLAGS) $(SOURCE)/level.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/level.o

# textures.o
$(ODIR)/textures.o: $(SOURCE)/textures.c $(SOURCE)/textures.h
	$(CC) $(CFLAGS) $(CWARNFLAGS) $(SOURCE)/textures.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/textures.o

# objects.o
$(ODIR)/objects.o: $(SOURCE)/objects.c $(SOURCE)/objects.h
	$(CC) $(CFLAGS) $(CWARNFLAGS) $(SOURCE)/objects.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/objects.o


clean:
	del $(ODIR)\*.o bin\main.exe
