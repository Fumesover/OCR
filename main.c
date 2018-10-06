#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "image.h"

int main(int argc, char **argv)
{
	char image[8] = "test.png";
	//DisplayImage(image);

	Uint32 **pixels = NULL;
	LoadImage(image, pixels);

	for (int i = 0; i < sizeof(pixels); i++)
		free(pixels[i]);

	free(pixels);
}

