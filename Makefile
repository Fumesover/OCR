# Makefile

# Setting the compiler and the default linker program
CC = gcc
# Options for pre-processor
CPPFLAGS = -MMD
# Main compilation options
CFLAGS = -g2 -Wall -Wextra -std=c99 -rdynamic -export-dynamic $(pkg-config --cflags sdl2) `pkg-config --cflags --libs gtk+-3.0` 
# Linker options
LDFLAGS = 
# Libs and path for linker
LDLIBS = `pkg-config --libs sdl2` `pkg-config --libs gtk+-3.0` -lSDL2 -lSDL2_image -lm -export-dynamic -rdynamic

SRC = imageTreatment/image.c imageTreatment/segmentation.c imageTreatment/matrix.c imageTreatment/queue.c neuralNetwork/neuralNet.c neuralNetwork/fromqueue.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: minicli #GUI/interface 

GUI/interface: ${OBJ} GUI/interface.o

run: all
	./bin/main

#imagetreatment:
#	${MAKE} -C imageTreatment 

xor: 
	${MAKE} -C neuralNetwork xor

minicli: ${OBJ} minicli.o

.PHONY: clean
clean:
	${RM} *.o */*.o	  # remove object files
	${RM} *.d */*.d   # remove dependency files
	${RM} main   	  # remove programs
	${RM} vgcore.*    
	${RM} xor 
	${RM} NNgraphviz
	${RM} minicli
	${RM} -r bin
	${RM} {*,*/*}.nn
	${MAKE} -C neuralNetwork clean 
	${MAKE} -C imageTreatment clean
	${RM} GUI/interface

-include ${DEP}

# END
