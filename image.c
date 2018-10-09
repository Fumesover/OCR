#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "image.h"


void DisplayImage(SDL_Surface *image)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *screen = SDL_CreateWindow("SDL2 Displaying Image",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1600, 1200, 0);

    SDL_Rect dest = { 1600/2 - image->w/2,1200/2 - image->h/2, 0, 0};
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
    Pixel **pixels = NULL; // To receive RGB value of the pixels of the image
    int **matrix = NULL; // Receives 0 and 1 considering the color (black/white) of the pixels of the image

	// Init matrix
	h = image->h;
	w = image->w;
	pixels = malloc(sizeof(Pixel*) * h);
	matrix = malloc(sizeof(Pixel*) * h);
	
	for (i = 0; i < h; i++)
	{
		pixels[i] = malloc(sizeof(Pixel) * w);
        matrix[i] = malloc(sizeof(Pixel) * w);
	}

    // Fill the martix
    FillPixels(pixels, image, h, w);

	//Greyscale
	GreyScale(pixels, h, w);
	DisplayImage(MatrixToSurface(pixels, h, w));

	// Otsu method on matrix
	int threshold = Otsu(pixels, h, w);
	Binarization(pixels, h, w, threshold);
    DisplayImage(MatrixToSurface(pixels, h, w));

    BinarizeMatrix(pixels, matrix, h, w);

    for (int i = 0; i < h; i++)
    {
        free(pixels[i]);
        free(matrix[i]);
    }

    free(pixels);
    free(matrix);
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
    SDL_LockSurface(image);

    int x = 0, y = 0;

    for (y = 0; y < h; y++)
    {
        for (x = 0; x < w; x++)
        {
            SDL_GetRGB(GetPixel(image, x, y), image->format, &pixels[y][x].r, &pixels[y][x].g, &pixels[y][x].b);
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
            break;

        case 2:
            return *(Uint16 *)p;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
            break;

        case 4:
            return *(Uint32 *)p;
            break;

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
int Otsu(Pixel **pixels, int h, int w)
{
        /**** HISTOGRAM ****/
    double probability[256], omega[256], mean[256], sigma[256];
    int histogram[256];
    int actual_color, total = 0;
    double max_sigma = 0.0;
    int threshold = 0;

    // Init the arrays
    for (int i = 0; i < 256; i++)
    {
        histogram[i] = 0;
        probability[i] = 0;
    }

    // Get the pixel colors histogram
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            actual_color = pixels[i][j].r;
            histogram[actual_color]++;
            total++;
        }
    }

    //Fill probability table
    for (int i = 0; i < 255; i++)
        probability[i] = (double)histogram[i] / total;

    omega[0] = probability[0];
    mean[0] = 0.0;

    for (int i = 1; i < 256; i++)
    {
        omega[i] = omega[i - 1] + probability[i];
        mean[i] = mean[i - 1] + i * probability[i];
    }

    for (int i = 0; i < 256;i++)
    {
        if (omega[i] != 0.0 && omega[i] != 1.0)
            sigma[i] = pow(mean[255]*omega[i] - mean[i], 2) /
                       (omega[i]*(1.0 - omega[i]));
        else
            sigma[i] = 0.0;
        if (sigma[i] > max_sigma) {
            max_sigma = sigma[i];
            threshold = i;
        }
    }

    return threshold;
}

// Binarize the matrix considering the threshold
void Binarization(Pixel **pixels, int h, int w, int threshold)
{
    int r, g, b;
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            r = pixels[i][j].r;
            g = pixels[i][j].g;
            b = pixels[i][j].b;
            if (r > threshold)
                pixels[i][j] = (Pixel){.r = 255, .g = 255, . b = 255};
            else
                pixels[i][j] = (Pixel){.r = 0, .g = 0, . b = 0};
        }
    }
}

void BinarizeMatrix(Pixel **pixels, int **binarized, int h, int w)
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            if (pixels[i][j].r == 0)
                binarized[i][j] = 0;

            else
                binarized[i][j] = 1;
        }
    }
}

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
            PutPixel(surface, j, i, pixel);
        }
    }

    return surface;
}
