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
LDLIBS = -lm $(pkg-config --libs sdl2) `pkg-config --libs gtk+-3.0` # -lm includes math library

SRC = main.c neuralNetwork/neuralNet.c 
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

main: ${OBJ}

valgrind: CFLAGS += -g
valgrind: clean
valgrind: main
valgrind:
	valgrind ./main

run: all
	./main

neuralNetwork/xor: neuralNetwork/xor.o neuralNetwork/neuralNet.o

NNgraphviz: neuralNetwork/NNgraphviz.o neuralNetwork/neuralNet.o

.PHONY: clean
clean:
	${RM} *.o */*.o	  # remove object files
	${RM} *.d */*.d   # remove dependency files
	${RM} main   	  # remove programs
	${RM} neuralNetwork/xor
	${RM} neuralNetwork/NNgraphviz
	${RM} vgcore.*    

-include ${DEP}

# END
