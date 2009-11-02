
####### Compiler, tools and options
CC       = gcc -DGNU #-DBIGENDIAN #-dr
#CC       = gcc -D DEBUG -D VERBOSE -g
#CC       = gcc -DGNU -g
ARCH     = -march=pentium4 -mmmx -msse -msse2 -mfpmath=sse -march=pentium4
#ARCH	= -march=native -mfpmath=sse 
OPTIM    = -O3 -fomit-frame-pointer -finline-functions -finline-functions-called-once -fno-math-errno -fdata-sections
CFLAGS   = -Wall -W  -combine -pipe $(ARCH) $(OPTIM)
LINK     = gcc
INCPATH  = -I. 
LIBS     = -lm

TAR      = tar -cf
GZIP     = gzip -9f
COPY     = cp -f
COPY_FILE= $(COPY)
COPY_DIR = $(COPY) -r
DEL_FILE = rm -f
SYMLINK  = ln -sf
DEL_DIR  = rmdir
MOVE     = mv -f
CHK_DIR_EXISTS= test -d
MKDIR    = mkdir -p

####### Output directory

OBJECTS_DIR = ./

####### Files

HEADERS = 	frame_read.h \
		generic_functions.h \
		limits.h \
		structures.h \
		files.h \
		initializations.h \
		types_processing.h \
		camp2ascii.h
SOURCES = 	generic_functions.c \
		frame_read.c \
		files.c \
		initializations.c \
		types_processing.c \
		camp2ascii.c
OBJECTS = 	generic_functions.o \
		frame_read.o \
		files.o \
		initializations.o \
		types_processing.o \
		camp2ascii.o
DESTDIR  = 
TARGET   = camp2ascii

first: all
####### Implicit rules

.SUFFIXES: .c .o 

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(UICDECLS) $(OBJECTS) $(OBJMOC)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJMOC) $(LIBS) $(OBJCOMP)

dist: 
	@mkdir -p .tmp/camp2ascii && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) .tmp/camp2ascii/ && $(COPY_FILE) .tmp/pptools/ && ( cd `dirname .tmp/camp2ascii` && $(TAR) camp2ascii.tar camp2ascii && $(GZIP) camp2ascii.tar ) && $(MOVE) `dirname .tmp/camp2ascii`/camp2ascii.tar.gz . && $(DEL_FILE) -r .tmp/camp2ascii


clean: 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) $(TARGET)

FORCE:

####### Compile

camp2ascii.o: camp2ascii.c camp2ascii.h files.h files.c

frame_read.o: frame_read.c frame_read.h structures.h types_processing.h types_processing.c

generic_functions.o: generic_functions.c generic_functions.h

files.o: files.c files.h

initializations.o: initializations.c initializations.h

types_processing.o: types_processing.c types_processing.h generic_functions.h

####### Install

install: all  

uninstall:  

