# Makefile

# Setting the compiler and the default linker program
CC = gcc
# Options for pre-processor
CPPFLAGS = -MMD
# Main compilation options
CFLAGS = -g -Wall -Wextra -Werror -std=c99 -O3 -march=native $(pkg-config --cflags sdl2) `pkg-config --cflags --libs gtk+-3.0`
# Linker options
LDFLAGS = 
# Libs and path for linker
LDLIBS = `pkg-config --libs sdl2` `pkg-config --libs gtk+-3.0` -lSDL2 -lSDL2_image -lm

SRC = imageTreatment/main.c imageTreatment/image.c imageTreatment/segmentation.c imageTreatment/matrix.c imageTreatment/queue.c neuralNetwork/neuralNet.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: bins

bins: imagetreatment xor
	mkdir -p bin
	mv neuralNetwork/xor imageTreatment/main bin

run: all
	./bin/main

imagetreatment:
	${MAKE} -C imageTreatment 

xor: 
	${MAKE} -C neuralNetwork xor

.PHONY: clean
clean:
	${RM} *.o */*.o	  # remove object files
	${RM} *.d */*.d   # remove dependency files
	${RM} main   	  # remove programs
	${RM} vgcore.*    
	${RM} xor 
	${RM} NNgraphviz
	${RM} -r bin
	${RM} {*,*/*}.nn
	${MAKE} -C neuralNetwork clean 
	${MAKE} -C imageTreatment clean

-include ${DEP}

# END
