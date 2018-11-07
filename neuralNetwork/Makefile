# Makefile

# Setting the compiler and the default linker program
CC = gcc

# Options for pre-processor
CFLAGS = -g -Wall -Wextra -Werror -std=c99 -march=native # compilation flags
CFLAGS += $(pkg-config --cflags sdl2) `pkg-config --cflags --libs gtk+-3.0`

# Linker options
LDFLAGS =

# Libs and path for linker
LDLIBS = -lm $(pkg-config --libs sdl2) `pkg-config --libs gtk+-3.0`

#########################################

all: xor

valgrind: CFLAGS += -g
valgrind: clean
valgrind: xor
valgrind:
	valgrind ./xor

run: all
	./xor

xor: xor.o neuralNet.o DataSet.o

NNtest: NNtest.o DataSet.o neuralNet.o

remspaces: clean
	sed -i 's/[ \t]*$$//' *.* Makefile

NNtrainWdata: NNtrainWdata.o neuralNet.o DataSet.o

.PHONY: clean
clean:
	${RM} *.o 		  	 # remove object files
	${RM} *.d   	     # remove dependency files
	${RM} NNgraphviz xor # remove main program
	${RM} xorGraphviz xor.gif
	${RM} {NN,}test
	${RM} vgcore.*		 # remove valgrind files
	${RM} *.nn  		 # remove neural network files

-include ${DEP}

# END
