#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "image.h"
#include "matrix.h"
#include "queue.h"

#define MIN_PIXELS = 10

// Displays the image "image" on screen
void DisplayImage(SDL_Surface *image)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *screen = SDL_CreateWindow("SDL2 Displaying Image",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 1000, 0);

    SDL_Rect dest = { 1000/2 - image->w/2,1000/2 - image->h/2, 0, 0};
    SDL_BlitSurface(image,NULL,SDL_GetWindowSurface(screen),&dest);

    SDL_UpdateWindowSurface(screen);
    SDL_Delay(500);

    SDL_FreeSurface(image);
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
void LoadImage(SDL_Surface *image)
{
	int i = 0;
	int h = 0, w = 0;
    Pixel **pixels = NULL; // To receive RGB value of the pixels of the image
    int **matrix = NULL; // Receives 0 and 1 considering the color of pixel

    /*** INIT ***/
	h = image->h;
	w = image->w;
	pixels = malloc(sizeof(Pixel*) * h);
	matrix = malloc(sizeof(int*) * h);

	for (i = 0; i < h; i++)
	{
		pixels[i] = malloc(sizeof(Pixel) * w);
        matrix[i] = malloc(sizeof(int) * w);
	}

    // Fill the martix
    SurfaceToMatrix(pixels, image, h, w);

	/*** BINARIZATION ***/
	//Greyscale
	GreyScale(pixels, h, w);
    //DisplayImage(MatrixToSurface(pixels, h, w));

	// Otsu method on matrix
	int threshold = Otsu(pixels, h, w);
	Binarization(pixels, h, w, threshold);

    BinarizeMatrix(pixels, matrix, h, w);
    //DisplayImage(MatrixToSurface(pixels, h, w));

    /*** SEGMENTATION ***/
    Segmentation(matrix, h, w);


    /*** FREE ALLOCATED MEMORY ***/
    for (int j = 0; j < h; j++)
    {
        free(pixels[j]);
        free(matrix[j]);
    }

    free(pixels);
    free(matrix);

}

// Fills matrix "pixels" with color values of
void SurfaceToMatrix(Pixel **pixels, SDL_Surface *image, int h, int w)
{
    SDL_LockSurface(image);

    int x = 0, y = 0;

    for (y = 0; y < h; y++)
    {
        for (x = 0; x < w; x++)
        {
            SDL_GetRGB(GetPixel(image, x, y), image->format, &pixels[y][x].r,
            &pixels[y][x].g, &pixels[y][x].b);
        }
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
            grey = (Uint8)(0.212 * pixels[i][j].r + 0.715 * pixels[i][j].g
                           + 0.072 * pixels[i][j].b);
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

void BinarizeMatrix(Pixel **pixels, int **binarized, int h, int w)
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            if (pixels[i][j].r == 0)
                binarized[i][j] = 1;

            else
                binarized[i][j] = 0;
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

    surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 
                                   rmask, gmask, bmask, amask);
    if(surface == NULL) {
        fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
        exit(1);
    }

    // Cast ARGB color Uint8 values into 1 Uint32 value
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            pixel = alpha << 24 | pixels[i][j].r << 16 | pixels[i][j].g << 8 
                                | pixels[i][j].b;
            PutPixel(surface, j, i, pixel);
        }
    }

    return surface;
}

// Main document segmentation functions
// Makes histogram and calls line segmentation function
void Segmentation(int **matrix, int h, int w)
{
    int *histo = NULL;
    Pixel **pixels = malloc(sizeof(Pixel*) * h);

    for (int i = 0; i < h; i++)
        pixels[i] = malloc(sizeof(Pixel) * w);

    /*** INIT ***/
    histo = malloc(sizeof(int) * h);

    Queue *queue = NULL;
    queue = malloc(sizeof(*queue));
    queue->first = NULL;

    InitList(histo, h);
    MatrixHHistogram(matrix, histo, h, w);

    /*** LINE SEGMENTATION ***/
    CutInLine(matrix, histo, queue, h,  w);

    // TEST: displays result
    //ShowSegmentation(queue);
    BinToPixels(matrix, pixels, h, w);
    DisplayImage(MatrixToSurface(pixels, h, w));

    /*** FREE ALLOCATED MEMORY ***/
    free(histo);

    for (int i = 0; i < h; i++)
        free(pixels[i]);
    free(pixels);
}

void CutInLine(int **matrix, int *histogram, Queue *queue, int h, int w)
{
    int i = 0, x1, x2;
    int *histoW = NULL;
    int **eol = NULL;
    Tuple *data = NewTuple();
    data->height = 1;
    data->width = 1;

    histoW = malloc(sizeof(int) * w);

    eol = malloc(sizeof(int*) * 1);
    eol[0] = malloc(sizeof(int) * 1);
    eol[0][0] = 10;

    data->data = eol;

    while (i < h)
    {
        if (histogram[i] >= 1)
        {
            if (i == 0)
                x1 = i;
            else
                x1 = i - 1;

            for (int j = 0; j < w; j++)
                matrix[x1][j] = 2;

            while (histogram[i] >= 1)
                i++;

            if (i == h - 1)
                x2 = i;
            else
                x2 = i + 1;

            for (int j = 0; j < w; j++)
                matrix[x2][j] = 2;

            //Creates histogram for each detected line
            MatrixWHistogram(matrix, histoW, x1, x2, w);

            //Cut line in char
            CutInChar(matrix, histoW, queue, x1, x2, w);

            //Add eol in list
            Enqueue(queue, data);
        }

        else
            i++;
    }

    free(histoW);

    free(eol[0]);
    free(eol);
}

void CutInChar(int **matrix, int *histogram, Queue *queue, int h1, int h2, int w)
{
    int i = 0, space = 0, x1 = 0, x2 = 0;
    int min_sp = 100, max_sp = 0;
    float average_sp = 0.0;

    int **sp = NULL;
    Tuple *data = NewTuple();
    data->height = 1;
    data->width = 1;

    sp = malloc(sizeof(int*) * 1);
    sp[0] = malloc(sizeof(int) * 1);
    sp[0][0] = 32;

    data->data = sp;

    // Finds the average space size of the line
    while (i < w)
    {
        if (histogram[i] == 0)
        {
            while (histogram[i] == 0)
            {
                i++;
                space++;
            }

            if (space < min_sp)
                min_sp = space;
            if (space > max_sp)
                max_sp = space;
        }

        else
            i++;

        space = 0;
    }

    i = 0;

    average_sp = (float)(min_sp + max_sp) / 2;

    while (i < w)
    {
        if (histogram[i] > 0)
        {
            if (i == 0)
                x1 = i;
            else
                x1 = i - 1;

            for (int j = h1; j < h2; j++)
                matrix[j][x1] = 3;

            while (histogram[i] > 0)
                i++;

            if (i == w - 1)
                x2 = i;
            else
                x2 = i + 1;

            for (int j = h1; j < h2; j++)
                matrix[j][x2] = 3;

            space = x2 - x1;
            if (space > average_sp)
                Enqueue(queue, data);

            EnqueueMatrix(matrix, queue, h1, h2, x1, x2);
        }

        else
            i++;
    }
}


void EnqueueMatrix(int **matrix, Queue *queue, int h1, int h2, int w1, int w2)
{
    int **new = NULL;
    new = malloc(sizeof(int *) * (h2 - h1));
    for (int i = 0; i < (h2-h1); i++)
        new[i] = malloc(sizeof(int) * (w2-w1));

    for (int i = h1; i < h2; i++)
    {
        for (int j = w1; j < w2; j++)
        {
            new[i-h1][j-w1] = matrix[i][j];
        }
    }

    PrintMatrix(new, h2-h1, w2-w1);
    Tuple *data = NewTuple();

    data->data = new;
    data->height = h2-h1;
    data->width = w2-w1;
    Enqueue(queue, data);
}


void ShowSegmentation(Queue *queue)
{
    Elt *curr = NULL;
    int **c;
    Pixel **m;

    if (queue->first != NULL)
        curr = queue->first;

    while (curr != NULL && curr->data != NULL)
    {
        if (curr->data != NULL) {
            c = curr->data->data;

            if (c[0][0] == 10)
                printf("\n");
            else if (c[0][0] == 32)
                printf(" ");
            else if (curr->data->width > 1 && curr->data->height > 1){
                printf("v");

                int h = curr->data->height;
                int w = curr->data->width;
                m = malloc(sizeof(Pixel*) * h);

                BinToPixels(c, m, h, w);
                DisplayImage(MatrixToSurface(m, h, w));

                //PrintMatrix(c, h, w);

                free(m);
            }
            else
                printf("\n");

            curr = curr->next;
        }
    }
}

// Fills the Pixel matrix considering the values of the binarized matrix
void BinToPixels(int **matrix, Pixel **pixels, int h, int w)
{
    for (int i = 0; i < h; i++)
        pixels[i] = malloc(sizeof(Pixel) * w);

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            switch(matrix[i][j])
            {
                case(1):
                    pixels[i][j].r = (Uint8)0;
                    pixels[i][j].g = (Uint8)0;
                    pixels[i][j].b = (Uint8)0;
                    break;
                case (0):
                    pixels[i][j].r = (Uint8)255;
                    pixels[i][j].g = (Uint8)255;
                    pixels[i][j].b = (Uint8)255;
                    break;
                case(2):
                    pixels[i][j].r = (Uint8)0;
                    pixels[i][j].g = (Uint8)255;
                    pixels[i][j].b = (Uint8)0;
                    break;
                case(3):
                    pixels[i][j].r = (Uint8)0;
                    pixels[i][j].g = (Uint8)0;
                    pixels[i][j].b = (Uint8)255;
                    break;
                default:
                    break;
            }
        }
    }
}