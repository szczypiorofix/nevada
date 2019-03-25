## My makefile
CC=gcc
CFLAGS=-c
CWARNFLAGS=-Wall -Wextra -Wpedantic #-Werror
ODIR=obj
SOURCE=src
OUTPUT=nevada.exe
LIBRARYPATH=-LC:\\mingw_dev_lib\\lib
INCLUDEPATH=-IC:\\mingw_dev_lib\\include
LINKERFLAGS=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -llibxml2 -liconv -llua -static-libgcc -static-libstdc++
## Optimize, -mwindows -no console output
OPTIMIZEFLAG=-O0 -ggdb
NOCONSOLE=-mwindows

debug: bin/debug/$(OUTPUT)

release: bin/release/$(OUTPUT)

# debug
bin/debug/$(OUTPUT): $(ODIR)/main.o $(ODIR)/defines.o $(ODIR)/engine.o $(ODIR)/level.o $(ODIR)/textures.o $(ODIR)/objects.o $(ODIR)/luac.o $(ODIR)/compare.o
	$(CC) $(CWARNFLAGS) $(OPTIMIZEFLAG) $(ODIR)/*.o $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o bin/debug/$(OUTPUT)


# release
bin/release/$(OUTPUT): $(ODIR)/main.o $(ODIR)/defines.o $(ODIR)/engine.o $(ODIR)/level.o $(ODIR)/textures.o $(ODIR)/objects.o $(ODIR)/compare.o
	$(CC) $(NOCONSOLE) $(ODIR)/*.o $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o bin/release/$(OUTPUT)



# main.o
$(ODIR)/main.o: $(SOURCE)/main.c $(SOURCE)/main.h $(SOURCE)/engine.c $(SOURCE)/engine.h $(SOURCE)/objects.c $(SOURCE)/objects.h $(SOURCE)/textures.c $(SOURCE)/textures.h $(SOURCE)/defines.h
	$(CC) $(CFLAGS) $(OPTIMIZEFLAG) $(CWARNFLAGS) $(SOURCE)/main.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/main.o


# defines.o
$(ODIR)/defines.o: $(SOURCE)/defines.c $(SOURCE)/defines.h
	$(CC) $(CFLAGS) $(OPTIMIZEFLAG) $(CWARNFLAGS) $(SOURCE)/defines.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/defines.o


# engine.o
$(ODIR)/engine.o: $(SOURCE)/engine.c $(SOURCE)/engine.h $(SOURCE)/defines.h $(SOURCE)/textures.h
	$(CC) $(CFLAGS) $(OPTIMIZEFLAG) $(CWARNFLAGS) $(SOURCE)/engine.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/engine.o


# level.o
$(ODIR)/level.o: $(SOURCE)/level.c $(SOURCE)/level.h
	$(CC) $(CFLAGS) $(OPTIMIZEFLAG) $(CWARNFLAGS) $(SOURCE)/level.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/level.o


# textures.o
$(ODIR)/textures.o: $(SOURCE)/textures.c $(SOURCE)/textures.h
	$(CC) $(CFLAGS) $(OPTIMIZEFLAG) $(CWARNFLAGS) $(SOURCE)/textures.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/textures.o


# objects.o
$(ODIR)/objects.o: $(SOURCE)/objects.c $(SOURCE)/objects.h
	$(CC) $(CFLAGS) $(OPTIMIZEFLAG) $(CWARNFLAGS) $(SOURCE)/objects.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/objects.o

# luac.o
$(ODIR)/luac.o: $(SOURCE)/luac.c $(SOURCE)/luac.h
	$(CC) $(CFLAGS) $(OPTIMIZEFLAG) $(CWARNFLAGS) $(SOURCE)/luac.c $(LIBRARYPATH) $(INCLUDEPATH) $(LINKERFLAGS) -o $(ODIR)/luac.o


# compare.o
$(ODIR)/compare.o: $(SOURCE)/compare.asm
	nasm -fwin32 $(SOURCE)/compare.asm -o $(ODIR)/compare.o
