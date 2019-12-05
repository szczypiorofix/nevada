## My super-duper makefile

ODIR_DEBUG=obj\debug
ODIR_RELEASE=obj\release

SDIR=src
CFLAGS=-c

CC=gcc
CWARNFLAGS=-Wall -Wextra -Wpedantic #-Werror #Treat all warnings as errors
LIBRARYPATH=-LC:\\mingw_dev_lib\\lib
INCLUDEPATH=-IC:\\mingw_dev_lib\\include
LINKER_LIBS=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -llibxml2 -liconv -llua -static-libgcc -static-libstdc++

# FOR DEBUG
FOR_DEBUG=-O0 -ggdb

# FOR RELEASE
FOR_RELEASE=-O2


# DEFINED STANDARD COMMAND LINE
RUN_STM=gcc -Wall -Wextra -Wpedantic -std=c11
# DEFINED INCLUDES & LIBRARIES
CC_LIBS=$(LIBRARYPATH) $(INCLUDEPATH) $(LINKER_LIBS)


FILE_TO_COMPILE_DEBUG=$(ODIR_DEBUG)/main.o\
$(ODIR_DEBUG)/defines.o\
$(ODIR_DEBUG)/engine.o\
$(ODIR_DEBUG)/luac.o\
$(ODIR_DEBUG)/font.o\
$(ODIR_DEBUG)/compare.o\
# $(ODIR_DEBUG)/res.o\


FILE_TO_COMPILE_RELEASE=$(ODIR_RELEASE)/main.o\
$(ODIR_RELEASE)/defines.o\
$(ODIR_RELEASE)/engine.o\
$(ODIR_RELEASE)/luac.o\
$(ODIR_RELEASE)/font.o\
$(ODIR_RELEASE)/compare.o\
# $(ODIR_RELEASE)/res.o\


# STARTING TARGETS
debug: bin/debug/nevada.exe
release: bin/release/nevada.exe


# debug
bin/debug/nevada.exe: $(FILE_TO_COMPILE_DEBUG)
	$(RUN_STM) $(FOR_DEBUG) $^ $(CC_LIBS) -o $@

# release
 bin/release/nevada.exe: $(FILE_TO_COMPILE_RELEASE)
	$(RUN_STM) $(FOR_RELEASE) $^ $(CC_LIBS) -o $@



# ALL FILES BUT NO ADDITIONAL PREREQUISITES
$(ODIR_DEBUG)/%.o: $(SDIR)/%.c
	$(RUN_STM) $(FOR_DEBUG) $(CFLAGS) $< $(CC_LIBS) -o $@

$(ODIR_RELEASE)/%.o: $(SDIR)/%.c
	$(RUN_STM) $(FOR_RELEASE) $(CFLAGS) $< $(CC_LIBS) -o $@


# compare.o
$(ODIR_DEBUG)/compare.o: $(SDIR)/compare.asm
	nasm -fwin32 $< -o $@
$(ODIR_RELEASE)/compare.o: $(SDIR)/compare.asm
	nasm -fwin32 $< -o $@

# res.o
$(ODIR_DEBUG)/res.o: nevada.rc
	windres -i nevada.rc -o $(ODIR_DEBUG)/res.o -O coff
$(ODIR_RELEASE)/res.o: nevada.rc
	windres -i nevada.rc -o $(ODIR_RELEASE)/res.o -O coff
