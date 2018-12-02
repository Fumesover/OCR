#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "image.h"
#include "matrix.h"
#include "segmentation.h"
#include "segmentation.h"

#define MIN_PIXELS = 10

// Displays the image "image" on screen
void DisplayImage(SDL_Surface *image)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *screen = SDL_CreateWindow("SDL2 Displaying Image",
       SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, image->w, image->h, 0);

    SDL_BlitSurface(image,NULL,SDL_GetWindowSurface(screen), 0);

    SDL_UpdateWindowSurface(screen);
    WaitForKeypressed();

    SDL_FreeSurface(image);

    SDL_DestroyWindow(screen);
}

void WaitForKeypressed()
{
    SDL_Event event;
    // Infinite loop, waiting for event
    for (;;) {
        // Take an event
        SDL_PollEvent( &event );
        // Switch on event type
        switch (event.type) {
            // Someone pressed a key -> leave the function
            case SDL_KEYDOWN: return;
            default: break;
        }
        // Loop until we got the expected event
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

// Load the image called 'path_image' int a Pixel matrix
SDL_Surface *LoadImage(char *path)
{
    SDL_Surface *image = NULL;
    image = IMG_Load(path);

    if (image == NULL)
    {
        fprintf(stderr, "Couldn't load %s: %s\n", path, SDL_GetError());
    }

	return image;
}


// Returns the greyscale value of the matrix of size h * w
void GreyScale(Pixel **pixels, int h, int w)
{
    Uint8 grey;
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            grey = (Uint8)(0.212 * pixels[i][j].r + 0.715 * pixels[i][j].g
                           + 0.072 * pixels[i][j].b);

            // Same grey value for all 3 RGB components
            pixels[i][j].r = grey;
            pixels[i][j].g = grey;
            pixels[i][j].b = grey;
        }
    }
}

// Returns the thresholded result of Pixel matrix
int Otsu(Pixel **pixels, int h, int w)
{
    /*** INIT ***/
    double probability[256], omega[256], mean[256], sigma[256];
    int histogram[256];
    int actual_color, total = 0;
    double max_sigma = 0.0;
    int threshold = 0;

    InitArray(histogram, 256);
    InitArray((int*)probability, 256);

    /**** FILLS HISTOGRAMS ****/
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

// Binarize the pixel matrix considering the threshold
void OtsuBinarization(Pixel **pixels, int h, int w, int threshold)
{
    int r;
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            r = pixels[i][j].r;
            if (r > threshold)
                pixels[i][j] = (Pixel){.r = 255, .g = 255, . b = 255};
            else
                pixels[i][j] = (Pixel){.r = 0, .g = 0, . b = 0};
        }
    }
}

// Binarize the int matrix considering the values of the pixels
void BinarizeMatrix(Pixel **pixels, int **binarized, int h, int w)
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            // 0 for white
            if (pixels[i][j].r == 0)
                binarized[i][j] = 1;

            // 1 for black
            else
                binarized[i][j] = 0;
        }
    }
}

// Fills the Pixel matrix considering the values of the binarized matrix
void BinToPixels(int **matrix, Pixel **pixels, int h, int w)
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            switch(matrix[i][j])
            {
                case(1): // BLACK
                case(4):
                    pixels[i][j].r = (Uint8)0;
                    pixels[i][j].g = (Uint8)0;
                    pixels[i][j].b = (Uint8)0;
                    break;
                case (0): // WHITE
                    pixels[i][j].r = (Uint8)255;
                    pixels[i][j].g = (Uint8)255;
                    pixels[i][j].b = (Uint8)255;
                    break;
                case(2): // LINES
                    pixels[i][j].r = (Uint8)255;
                    pixels[i][j].g = (Uint8)204;

                    pixels[i][j].b = (Uint8)0;
                    break;
                case(3): // CHARACTERS
                    pixels[i][j].r = (Uint8)68;
                    pixels[i][j].g = (Uint8)0;
                    pixels[i][j].b = (Uint8)255;
                    break;
                default:
                    pixels[i][j].r = (Uint8)105;
                    pixels[i][j].g = (Uint8)105;
                    pixels[i][j].b = (Uint8)105;
                    break;
            }
        }
    }
}


// Fills matrix "pixels" with color values of
void SurfaceToMatrix(Pixel **pixels, SDL_Surface *image, int h, int w)
{
    // GetPixel may fail if the surface is unlocked
    SDL_LockSurface(image);

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            SDL_GetRGB(GetPixel(image, x, y), image->format, &pixels[y][x].r,
                       &pixels[y][x].g, &pixels[y][x].b);
        }
    }
}

// Returns the surface matching the Pixel matrix
SDL_Surface *MatrixToSurface(Pixel **pixels, int h, int w)
{
    /*** INIT ***/
    SDL_Surface *surface;
    Uint32 pixel = 0;
    Uint32 alpha = 255;
    Uint32 rmask, gmask, bmask, amask;

    // The mask depends on the endianness (byte order) of the machine
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

    // Creates new surface
    surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 
                                   rmask, gmask, bmask, amask);
    if(surface == NULL) {
        fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
        exit(1);
    }


    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            // Cast ARGB color Uint8 values into 1 Uint32 value
            pixel = alpha << 24 | pixels[i][j].r << 16 | pixels[i][j].g << 8 
                                | pixels[i][j].b;

            // Puts this pixel in the surface
            PutPixel(surface, j, i, pixel);
        }
    }

    return surface;
}

// Converts the matrix "matrix" to a sdl_surface
// Displays the matrix on screen
void DisplayMatrix(int **matrix, int h, int w)
{
    Pixel **pixels = InitPixelMatrix(h, w);
    BinToPixels(matrix, pixels, h, w);
    SDL_Surface *surface = MatrixToSurface(pixels, h, w);
    DisplayImage(surface);

    //SDL_FreeSurface(surface);
    FreeMatrix((void**)pixels, h);
}


// Reducing noise in image after greyscale
// We can consider only 1 component of pixel (here .r)
void Median_Filter(Pixel **pixels, int h, int w)
{
	int neighbor_array[9] = {0,0,0,0,0,0,0,0,0};
	for(int i = 0; i < h; i ++)
	{
		for(int j = 0; j < w; j ++)
		{
			neighbor_array[0] = (int)(pixels[i][j].r);
			// Case pixel is in top border of matrix
			if(i == 0)
			{	
				neighbor_array[1] = (int)(pixels[i+1][j].r);
				// Upper left corner
				if(j == 0)
				{
					neighbor_array[2] = (int)(pixels[i][j+1].r);
					neighbor_array[3] = (int)(pixels[i + 1][j + 1].r);
					neighbor_array[4] = 0;
					neighbor_array[5] = 255;
				}
				// Upper right corner
				if(j == (w - 1))
				{
					neighbor_array[2] = (int)(pixels[i][j - 1].r);
					neighbor_array[3] = (int)(pixels[i + 1][j - 1].r);
					neighbor_array[4] = 0;
					neighbor_array[5] = 255;
				}
				// Upper middle
				else
				{
					neighbor_array[2] = (int)(pixels[i][j+1].r);
					neighbor_array[3] = (int)(pixels[i + 1][j + 1].r);
					neighbor_array[4] = (int)(pixels[i][j - 1].r);
					neighbor_array[5] = (int)(pixels[i + 1][j - 1].r);

				}
				neighbor_array[6] = 0;
				neighbor_array[7] = 0;
				neighbor_array[8] = 255;
			}

			// Case pixel is in bottom border of matrix
			if(i == (h - 1))
			{
				neighbor_array[1] = (int)(pixels[i - 1][j].r);
				// Case bottom left corner
				if(j == 0)
				{
					neighbor_array[2] = (int)(pixels[i - 1][j + 1].r);
					neighbor_array[3] = (int)(pixels[i][j + 1].r);
					neighbor_array[4] = 0;
					neighbor_array[5] = 255;
				}
				// Case bottom right corner
				if(j == (w - 1))
				{
					neighbor_array[2] = (int)(pixels[i - 1][j - 1].r);
					neighbor_array[3] = (int)(pixels[i][j - 1].r);
					neighbor_array[4] = 0;
					neighbor_array[5] = 255;
				}
				// Case bottom middle
				else
				{
					neighbor_array[2] = (int)(pixels[i - 1][j + 1].r);
					neighbor_array[3] = (int)(pixels[i][j + 1].r);
					neighbor_array[4] = (int)(pixels[i - 1][j - 1].r);
					neighbor_array[5] = (int)(pixels[i][j - 1].r);
				}
				neighbor_array[6] = 0;
				neighbor_array[7] = 0;
				neighbor_array[8] = 255;
			}
			
			// Case pixel is on left border
			if((j == 0) && (i != 0) && (i != (h - 1)))
			{
				neighbor_array[1] = (int)(pixels[i - 1][j].r);
				neighbor_array[2] = (int)(pixels[i - 1][j + 1].r);
				neighbor_array[3] = (int)(pixels[i][j + 1].r);
				neighbor_array[4] = (int)(pixels[i + 1][j + 1].r);
				neighbor_array[5] = (int)(pixels[i + 1][j].r);
				neighbor_array[6] = 0;
				neighbor_array[7] = 0;
				neighbor_array[8] = 255; 
			}
			
			// Case pixel is on right border
			if(j == (w - 1) && (i != 0) && (i != (h - 1)))
			{
				neighbor_array[1] = (int)(pixels[i - 1][j].r);
				neighbor_array[2] = (int)(pixels[i - 1][j - 1].r);
				neighbor_array[3] = (int)(pixels[i][j - 1].r);
				neighbor_array[4] = (int)(pixels[i + 1][j - 1].r);
				neighbor_array[5] = (int)(pixels[i + 1][j].r);
				neighbor_array[6] = 0;
				neighbor_array[7] = 0;
				neighbor_array[8] = 255; 
			}
			
			// Case pixel has 8 neighbors
			if((i != 0) && (j != 0) && (i != (h-1)) && (j != (w-1)) )
			{
				neighbor_array[1] = (int)(pixels[i][j + 1].r);
				neighbor_array[2] = (int)(pixels[i][j - 1].r);
				neighbor_array[3] = (int)(pixels[i + 1][j].r);

				neighbor_array[4] = (int)(pixels[i - 1][j].r);


				neighbor_array[5] = (int)(pixels[i + 1][j + 1].r);
				neighbor_array[6] = (int)(pixels[i - 1][j + 1].r);
			
				neighbor_array[7] = (int)(pixels[i + 1][j - 1].r);	
				neighbor_array[8] = (int)(pixels[i - 1][j - 1].r);
			}

			// Sort array
			mergeSort(neighbor_array, 0, 8);
			// Correct pixel value with median value in array
			pixels[i][j].r = neighbor_array[4];
			pixels[i][j].g = neighbor_array[4];
			pixels[i][j].b = neighbor_array[4];
		}
	}
}

// Merges two subarrays of arr[]. 
// First subarray is arr[l..m] 
// Second subarray is arr[m+1..r] 
void merge(int arr[], int l, int m, int r) 
{ 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
  
    /* create temp arrays */
    int L[n1], R[n2]; 
  
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++) 
        L[i] = arr[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1+ j]; 
  
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
    k = l; // Initial index of merged subarray 
    while (i < n1 && j < n2) 
    { 
        if (L[i] <= R[j]) 
        { 
            arr[k] = L[i]; 
            i++; 
        } 
        else
        { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  
    /* Copy the remaining elements of L[], if there 
       are any */
    while (i < n1) 
    { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 
  
    /* Copy the remaining elements of R[], if there 
       are any */
    while (j < n2) 
    { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
} 
  
/* l is for left index and r is right index of the 
   sub-array of arr to be sorted */
void mergeSort(int arr[], int l, int r) 
{ 
    if (l < r) 
    { 
        // Same as (l+r)/2, but avoids overflow for 
        // large l and h 
        int m = l+(r-l)/2; 
  
        // Sort first and second halves 
        mergeSort(arr, l, m); 
        mergeSort(arr, m+1, r); 
  
        merge(arr, l, m, r); 
    } 
} 
