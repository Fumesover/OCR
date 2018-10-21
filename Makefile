# Makefile

# Setting the compiler and the default linker program
CC = gcc
# Options for pre-processor
CPPFLAGS = -MMD
# Main compilation options
CFLAGS = -g -Wall -Wextra -Werror -std=c99 -O3 -march=native -rdynamic -export-dynamic $(pkg-config --cflags sdl2) `pkg-config --cflags --libs gtk+-3.0` 
# Linker options
LDFLAGS = 
# Libs and path for linker
LDLIBS = `pkg-config --libs sdl2` `pkg-config --libs gtk+-3.0` -lSDL2 -lSDL2_image -lm -export-dynamic -rdynamic

SRC = imageTreatment/image.c imageTreatment/segmentation.c imageTreatment/matrix.c imageTreatment/queue.c neuralNetwork/neuralNet.c GUI/interface.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: GUI/interface 

GUI/interface: ${OBJ}

valgrind: CFLAGS += -g
valgrind: clean
valgrind: main
valgrind:
	valgrind ./main

run: all
	./main

xor: 
	(cd neuralNetwork; make xor; mv xor ..)

.PHONY: clean
clean:
	${RM} *.o */*.o	  # remove object files
	${RM} *.d */*.d   # remove dependency files
	${RM} main   	  # remove programs
	${RM} vgcore.*    
	${RM} xor 
	${RM} NNgraphviz
	${MAKE} -C neuralNetwork clean

-include ${DEP}

# END
