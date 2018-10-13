# Makefile

# Setting the compiler and the default linker program
CC = gcc
# Options for pre-processor
CPPFLAGS = -MMD
# Main compilation options
CFLAGS = -Wall -Wextra -Werror -std=c99 -O3 -march=native $(pkg-config --cflags sdl2) `pkg-config --cflags --libs gtk+-3.0`
# Linker options
LDFLAGS =
# Libs and path for linker
LDLIBS = -lm $(pkg-config --libs sdl2) `pkg-config --libs gtk+-3.0` # -lm includes math library

SRC = xor.c neuralNet.c 
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: xor 

main: ${OBJ}

valgrind: CFLAGS += -g
valgrind: clean
valgrind: xor
valgrind:
	valgrind ./xor

run: all
	./xor

xor: xor.o neuralNet.o

NNgraphviz: NNgraphviz.o neuralNet.o

showgraph: NNgraphviz
	./NNgraphviz | dot -Tpng > test
	feh test
	rm test

.PHONY: clean
clean:
	${RM} *.o 		  	 # remove object files
	${RM} *.d   	     # remove dependency files
	${RM} NNgraphviz xor # remove main program
	${RM} vgcore.*

-include ${DEP}

# END
