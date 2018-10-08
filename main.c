#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "image.h"
#include "interface.h"

int main()
{
    // TODO : Select file from interface file chooser
    char *path_image = "/home/rhaeven/Documents/OCR/test.jpg";
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

