#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unistd.h>

#include "image.h"
#include "interface.h"
#include "matrix.h"
#include "queue.h"

#include "neuralNetwork/neuralNet.h"

int main(int argc, char** argv)
{
    // TODO : Select file from interface file chooser
    char *path_image = "/home/rhaeven/Documents/OCR/scan.jpg";
   
    int opt;
    while ((opt = getopt(argc, argv, "i:")) != -1) {
        switch (opt) {
            case 'i':
                path_image = optarg;
                break;
            default:
                break;
        }
    }


    SDL_Surface *image = IMG_Load(path_image);

    if (image == NULL)
    {
        fprintf(stderr, "Couldn't load %s: %s\n", path_image, SDL_GetError());
        return 1;
    }

    //DisplayImage(image);

	// Return the corresponding Pixel matrix of the image
	LoadImage(image);

    return 0;
}

