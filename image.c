#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "image.h"

/*
void DisplayImage(char path_image[]) 
{
    SDL_Surface *image;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Surface * screen = SDL_CreateWindow("SDL2 Displaying Image",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
*/
   /* Load the BMP file into a surface */
  /*  image = IMG_Load(path_image);
    if (image == NULL) {
        fprintf(stderr, "Couldn't load %s: %s\n", path_image, SDL_GetError());
        return;
    }
*/
    /*
     * Palettized screen modes will have a default palette (a standard
     * 8*8*4 colour cube), but if the image is palettized as well we can
     * use that palette for a nicer colour matching
     */
  /*  if (image->format->palette && screen->format->palette) {
    SDL_SetColors(screen, image->format->palette->colors, 0,
                  image->format->palette->ncolors);
    }
*/
    /* Blit onto the screen surface */
  /*  if(SDL_BlitSurface(image, NULL, screen, NULL) < 0)
        fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());

    SDL_UpdateRect(screen, 0, 0, image->w, image->h);
*/
    /* Free the allocated BMP surface */
  /*  SDL_FreeSurface(image);	
} */

// Charge l'image nommée 'image'
void LoadImage(char path_image[], Uint32 **pixels)
{
	int i = 0, j = 0;
	int h = 0, w = 0;
	SDL_Surface *image = NULL;

	image = IMG_Load(path_image);
	
	// Initialisation de la taille du tableau
	h = image->h;
	w = image->w;
	pixels = malloc(sizeof(Uint32) * h);
	
	for (i = 0; i < h; i++)
	{
		pixels[i] = malloc(sizeof(Uint32) * w);
	}

	// Remplissage du tableau
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			pixels[i][j] = GetPixel(image, i, j);
		}
	}	
}

// Renvoie la couleur du pixel à la position x, y
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

// Change la valeur du pixel x, y
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
