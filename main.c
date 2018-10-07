#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "image.h"
#include "interface.h"

int main(int argc, char **argv)
{
    // TODO : Select file from interface file chooser
	char image[] = "/home/rhaeven/Documents/OCR/test.jpg";

	// Return the corresponding Pixel matrix of the image
	LoadImage(image);

	return 0;
}

