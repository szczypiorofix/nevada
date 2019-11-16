## My super-duper makefile

ODIR=obj
SDIR=src
CFLAGS=-c

CC=gcc
CWARNFLAGS=-Wall -Wextra -Wpedantic #-Werror #Treat all warnings as errors
LIBRARYPATH=-LC:\\mingw_dev_lib\\lib
INCLUDEPATH=-IC:\\mingw_dev_lib\\include
LINKER_LIBS=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -llibxml2 -liconv -llua -static-libgcc -static-libstdc++
# DEFINED STANDARD COMMAND LINE
DEBUG_STM=gcc -Wall -Wextra -Wpedantic -O0 -ggdb -std=c11
# DEFINED INCLUDES & LIBRARIES
CC_LIBS=$(LIBRARYPATH) $(INCLUDEPATH) $(LINKER_LIBS)


FILE_TO_COMPILE=$(ODIR)/main.o\
$(ODIR)/defines.o\
$(ODIR)/engine.o\
$(ODIR)/luac.o\
$(ODIR)/font.o\
$(ODIR)/compare.o\
$(ODIR)/res.o\

# STARTING TARGETS
debug: bin/debug/nevada.exe
release: bin/release/nevada.exe


# debug
bin/debug/nevada.exe: $(FILE_TO_COMPILE)
	$(DEBUG_STM) $^ $(CC_LIBS) -o $@

# release
 bin/release/nevada.exe: $(FILE_TO_COMPILE)
	$(DEBUG_STM) $^ $(CC_LIBS) -o $@


# ALL FILES BUT NO ADDITIONAL PREREQUISITES
# $(ODIR)/%.o: $(SDIR)/%.c
#	$(DEBUG_STM) $(CFLAGS) $< $(CC_LIBS) -o $@



#main.o
$(ODIR)/main.o: $(SDIR)/main.c $(SDIR)/main.h
	$(DEBUG_STM) $(CFLAGS) $< $(CC_LIBS) -o $@

# defines.o
$(ODIR)/defines.o: $(SDIR)/defines.c $(SDIR)/defines.h
	$(DEBUG_STM) $(CFLAGS) $< $(CC_LIBS) -o $@

# engine.o
$(ODIR)/engine.o: $(SDIR)/engine.c $(SDIR)/engine.h
	$(DEBUG_STM) $(CFLAGS) $< $(CC_LIBS) -o $@

# luac.o
$(ODIR)/luac.o: $(SDIR)/luac.c $(SDIR)/luac.h
	$(DEBUG_STM) $(CFLAGS) $< $(CC_LIBS) -o $@

# font.o
$(ODIR)/font.o: $(SDIR)/font.c $(SDIR)/font.h
	$(DEBUG_STM) $(CFLAGS) $< $(CC_LIBS) -o $@

# compare.o
$(ODIR)/compare.o: $(SDIR)/compare.asm
	nasm -fwin32 $< -o $@

# res.o
$(ODIR)/res.o: nevada.rc
	windres -i nevada.rc -o $(ODIR)/res.o -O coff
