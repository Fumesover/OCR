# Makefile

# Setting the compiler and the default linker program
CC = gcc
# Options for pre-processor
CPPFLAGS = -MMD
# Main compilation options
CFLAGS = -g2 -Wall -Wextra -std=c99 -rdynamic -export-dynamic 
CFLAGS += $(pkg-config --cflags sdl2) `pkg-config --cflags --libs gtk+-3.0` 
# Linker options
LDFLAGS = 
# Libs and path for linker
LDLIBS = `pkg-config --libs sdl2` `pkg-config --libs gtk+-3.0` 
LDLIBS += -lSDL2 -lSDL2_image -lm -export-dynamic -rdynamic

SRC = imageTreatment/segmentation.c imageTreatment/matrix.c 
SRC += imageTreatment/queue.c neuralNetwork/neuralNet.c 
SRC += neuralNetwork/fromqueue.c imageTreatment/image.c 

OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: minicli GUI/interface 

run: all
	(cd GUI; ./interface)

GUI/interface: ${OBJ} GUI/interface.o

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
	${MAKE} -C neuralNetwork clean 
	${MAKE} -C imageTreatment clean
	${RM} GUI/interface

-include ${DEP}

# END
