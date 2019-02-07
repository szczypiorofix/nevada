## My makefile
CC=gcc
CFLAGS=-c
CWARNFLAGS=-Wall -Wextra -Wpedantic #-Werror
ODIR=obj
SOURCE=src
LIBRARYPATH=-LC:\\mingw_dev_lib\\lib
INCLUDEPATH=-IC:\\mingw_dev_lib\\include
LINKERFLAGS=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -llibxml2 -liconv -llua -static-libgcc -static-libstdc++
## Optimize, -mwindows -no console output
OPTIMIZEFLAG=-O0 -ggdb

debug: bin/debug/nevada.exe

release: bin/release/nevada.exe

# debug
bin/debug/nevada.exe: $(ODIR)/main.o $(ODIR)/defines.o $(ODIR)/engine.o $(ODIR)/arraylist.o $(ODIR)/list.o $(ODIR)/level.o $(ODIR)/textures.o $(ODIR)/objects.o
	$(CC) $(CWARNFLAGS) $(OPTIMIZEFLAG) $(ODIR)/main.o $(ODIR)/defines.o $(ODIR)/engine.o $(ODIR)/arraylist.o $(ODIR)/list.o $(ODIR)/level.o $(ODIR)/textures.o $(ODIR)/objects.o $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o bin/debug/nevada.exe


# release
bin/release/nevada.exe: $(ODIR)/main.o $(ODIR)/defines.o $(ODIR)/engine.o $(ODIR)/arraylist.o $(ODIR)/list.o $(ODIR)/level.o $(ODIR)/textures.o $(ODIR)/objects.o
	$(CC) $(ODIR)/main.o $(ODIR)/defines.o $(ODIR)/engine.o $(ODIR)/arraylist.o $(ODIR)/list.o $(ODIR)/level.o $(ODIR)/textures.o $(ODIR)/objects.o $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o bin/release/nevada.exe



# main.o
$(ODIR)/main.o: $(SOURCE)/main.c $(SOURCE)/main.h
	$(CC) $(CFLAGS) $(OPTIMIZEFLAG) $(CWARNFLAGS) $(SOURCE)/main.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/main.o

# defines.o
$(ODIR)/defines.o: $(SOURCE)/defines.c $(SOURCE)/defines.h
	$(CC) $(CFLAGS) $(OPTIMIZEFLAG) $(CWARNFLAGS) $(SOURCE)/defines.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/defines.o

# engine.o
$(ODIR)/engine.o: $(SOURCE)/engine.c $(SOURCE)/engine.h
	$(CC) $(CFLAGS) $(OPTIMIZEFLAG) $(CWARNFLAGS) $(SOURCE)/engine.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/engine.o

# arraylist.o
$(ODIR)/arraylist.o: $(SOURCE)/arraylist.c $(SOURCE)/arraylist.h
	$(CC) $(CFLAGS) $(OPTIMIZEFLAG) $(CWARNFLAGS) $(SOURCE)/arraylist.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/arraylist.o

# list.o
$(ODIR)/list.o: $(SOURCE)/list.c $(SOURCE)/list.h
	$(CC) $(CFLAGS) $(OPTIMIZEFLAG) $(CWARNFLAGS) $(SOURCE)/list.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/list.o

# level.o
$(ODIR)/level.o: $(SOURCE)/level.c $(SOURCE)/level.h
	$(CC) $(CFLAGS) $(OPTIMIZEFLAG) $(CWARNFLAGS) $(SOURCE)/level.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/level.o

# textures.o
$(ODIR)/textures.o: $(SOURCE)/textures.c $(SOURCE)/textures.h
	$(CC) $(CFLAGS) $(OPTIMIZEFLAG) $(CWARNFLAGS) $(SOURCE)/textures.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/textures.o

# objects.o
$(ODIR)/objects.o: $(SOURCE)/objects.c $(SOURCE)/objects.h
	$(CC) $(CFLAGS) $(OPTIMIZEFLAG) $(CWARNFLAGS) $(SOURCE)/objects.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/objects.o


clean:
	del $(ODIR)\*.o bin\debug\main.exe bin\release\main.exe
