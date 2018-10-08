#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "image.h"


void DisplayImage(SDL_Surface *image)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *screen = SDL_CreateWindow("SDL2 Displaying Image",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);

    SDL_Rect dest = { 640/2 - image->w/2,480/2 - image->h/2, 0, 0};
    SDL_BlitSurface(image,NULL,SDL_GetWindowSurface(screen),&dest);

    SDL_UpdateWindowSurface(screen);
    SDL_Delay(1000);


    /* Free the allocated surface */
    SDL_FreeSurface(image);
}

// Load the image called 'path_image' int a Pixel matrix
void LoadImage(SDL_Surface *image)
{
	int i = 0;
	int h = 0, w = 0;
    Pixel **pixels = NULL;

	// Init matrix
	h = image->h;
	w = image->w;
	pixels = malloc(sizeof(Pixel*) * h);
	
	for (i = 0; i < h; i++)
	{
		pixels[i] = malloc(sizeof(Pixel) * w);
	}

    // Fill the martix
    FillPixels(pixels, image, h, w);

	//TEST
	//PrintPixels(pixels, h, w);

	GreyScale(pixels, h, w);
	DisplayImage(MatrixToSurface(pixels, h, w));
	//PrintPixels(pixels, h, w);

    for (int i = 0; i < h; i++)
        free(pixels[i]);

    free(pixels);
}

// TEST THE MATRIX
// Prints the RGB values og the matrix of size h * w
void PrintPixels(Pixel **matrix, int h, int w)
{
    for (int x = 0; x < h; x++)
    {
        for (int y = 0; y < w; y++)
        {
            printf("(r: %u, g: %u, b: %u) ", matrix[x][y].r, matrix[x][y].g, matrix[x][y].b);
        }

        printf("\n");
    }
}

void FillPixels(Pixel **pixels, SDL_Surface *image, int h, int w)
{
    int x = 0, y = 0;

    for (x = 0; x < h; x++)
    {
        for (y = 0; y < w; y++)
        {
            SDL_GetRGB(GetPixel(image, x, y), image->format, &pixels[x][y].r, &pixels[x][y].g, &pixels[x][y].b);
        }
    }
}

// Returns the Uint32 value of the pixel at position x, y
Uint32 GetPixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

// Change value of pixel at position x, y
void PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

// Returns the greyscale value of the matrix of size h * w
void GreyScale(Pixel **pixels, int h, int w)
{
    Uint8 grey;
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            grey = (Uint8)(0.2126 * pixels[i][j].r + 0.7152 * pixels[i][j].g + 0.0722 * pixels[i][j].b);
            pixels[i][j].r = grey;
            pixels[i][j].g = grey;
            pixels[i][j].b = grey;
        }
    }
}

// Returns the thresholded result of Pixel matrix
/*void Otsu(Pixel **pixels)
{

}*/

SDL_Surface *MatrixToSurface(Pixel **pixels, int h, int w)
{
    SDL_Surface *surface;
    Uint32 pixel = 0;
    Uint32 alpha = 255;
    Uint32 rmask, gmask, bmask, amask;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    #else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    #endif

    surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, rmask, gmask, bmask, amask);
    if(surface == NULL) {
        fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
        exit(1);
    }

    // Cast ARGB color Uint8 values into 1 Uint32 value
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            pixel = alpha << 24 | pixels[i][j].r << 16 | pixels[i][j].g << 8 | pixels[i][j].b;
            PutPixel(surface, i, j, pixel);
        }
    }

    return surface;
}
