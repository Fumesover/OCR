#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "matrix.h"
#include "image.h"

#define SIZE 20

/*** Matrixes ***/

// Prints the values of the matrix into the terminal
void PrintMatrix(int **matrix, int h, int w)
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            printf(" %d ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("===============================================================\n");
}

// Initializes the matrix
int** InitIntMatrix(int h, int w)
{
    int **matrix = NULL;
    matrix = malloc(sizeof(int*) * h);
    for (int i = 0; i < h; i++)
        matrix[i] = malloc(sizeof(int) * w);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            matrix[i][j] = 0;
        }
    }

    return matrix;
}

void FreeMatrix(void **m, int h)
{
    for (int i = 0; i < h; i++)
        free(m[i]);
    free(m);
}

Pixel** InitPixelMatrix(int h, int w)
{
    Pixel **matrix = malloc(sizeof(Pixel*) * h);
    for (int i = 0; i < h; i++)
        matrix[i] = malloc(sizeof(Pixel) * w);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            matrix[i][j].r = 0;
            matrix[i][j].g = 0;
            matrix[i][j].b = 0;
        }
    }

    return matrix;
}

/*** Lists ***/

// Initializes all values of the list to 0
void InitArray(int *array, int h)
{
    for (int i = 0; i < h; i++)
        array[i] = 0;
}

// Prints the list of the matrix into the terminal
void PrintArray(int *list, int h)
{
    for (int i = 0; i < h; i++)
    {
        printf("(%d)=%d, ", i, list[i]);
        fflush(stdout);
    }
    printf("\n");
}

/*** Histograms of black pixels ***/

// Returns the histogram of all lines of the matrix
void MatrixHHistogram(int **matrix, int *histogram, int h, int w)
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            if (matrix[i][j] == 1)
                histogram[i]++;
        }
    }
}

// Returns the histogram of all columns of the matrix
// in the line range [h1, h2]
void MatrixWHistogram(int **matrix, int *histogram, int h1, int h2, int w)
{
    for (int x = 0; x < w; x++)
    {
        for (int y = h1; y < h2; y++)
        {
            if (matrix[y][x] == 1)
                histogram[x]++;
        }
    }
}

// Copy content of mat1 into mat2
void Copy(int **mat1, int**mat2, int h, int w)
{
    for (int i = 0; i < h; i++)
    {
         for (int j = 0; j < w; j++)
            mat2[i][j] = mat1[i][j];
    }
}

// Remove the white borders on the side of the matrix
int **RemoveWhite(int **matrix, int *h, int *w)
{
    /*** INIT ***/
    int* histoH = malloc(sizeof(int) * *h);
    int* histoW = malloc(sizeof(int) * *w);
    int** res;
    int y = 0, x = 0, rx = *w - 1, ry = *h - 1;

    InitArray(histoH, *h);
    InitArray(histoW, *w);

    MatrixHHistogram(matrix, histoH, *h, *w);
    MatrixWHistogram(matrix, histoW, 0, *h, *w);

    // Find boundaries of matrixes
    while (x < *w && histoW[x] == 0)
        x++;
    while (rx > 0 && histoW[rx] == 0)
        rx--;
    while (y < *h && histoH[y] == 0)
        y++;
    while (ry > 0 && histoH[ry] == 0)
        ry--;

    int resh = ry - y + 1, resw = rx - x + 1;
    res = InitIntMatrix(resh, resw);

    for (int i = 0; i < resh; i++) {
        for (int j = 0; j < resw; j++)
            res[i][j] = matrix[i + y][j + x];
    }

    *h = resh;
    *w = resw;

    return res;
}


// Returns a squared matrix equivalent to the original
// By adding white space on the thinner sides
int **SquareMatrix(int **matrix, int h, int w)
{
    /*** INIT ***/
    int t;

    if (h > w) t = h; else t = w;

    int** res = NULL;
    res = InitIntMatrix(t, t);

    int offset = (t - w) / 2;

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (matrix[i][j] == 1) {
                if (h > w)
                    res[i][j + offset] = matrix[i][j];
                else
                    res[i + offset][j] = matrix[i][j];
            }
        }
    }
    return res;
}

// Returns the matrix to the adequate format and size
int **Resize(int **matrix, int h, int w, int newsize)
{
    // Remove white borders of the matrix
    int **cut = RemoveWhite(matrix, &h, &w);
    // Squares matrix
    int** square = SquareMatrix(cut, h, w);

    int size = h > w ? h : w;

    Pixel** pix1 = InitPixelMatrix(size, size);
    Pixel** pix2 = InitPixelMatrix(newsize, newsize);

    // Conversions and resize
    BinToPixels(square, pix1, size, size);
    SDL_Surface *n = MatrixToSurface(pix1, size, size);
    SDL_Surface *res = ResizeSurface(n, (Uint16)newsize);

    int **final = InitIntMatrix(newsize, newsize);

    SurfaceToMatrix(pix2, res, newsize, newsize);
    BinarizeMatrix(pix2, final, newsize, newsize);

    FreeMatrix((void**)pix1, size);
    FreeMatrix((void**)cut, h);
    FreeMatrix((void**)pix2, newsize);
    FreeMatrix((void**)square, size);

    SDL_FreeSurface(res);
    SDL_FreeSurface(n);

    return final;
}

// Resize the matrix
SDL_Surface *ResizeSurface(SDL_Surface *Surface, int s)
{
    SDL_Surface *res = SDL_CreateRGBSurface(Surface->flags, (Uint16)s, (Uint16)s,
                                            Surface->format->BitsPerPixel,
                                            Surface->format->Rmask, Surface->format->Gmask,
                                            Surface->format->Bmask, Surface->format->Amask);

    int h = Surface->h;
    int w = Surface->w;

    double resizeX = ((double)(s)  / (double)(w));
    double resizeY = ((double)(s) / (double)(h));

    for(int y = 0; y < h; y++)
        for(int x = 0; x < w; x++)
            for(int i = 0; i < resizeY; i++)
                for(int j = 0; j < resizeX; j++)
                    PutPixel(res, (int)(resizeX * x) + j,
                             (int)(resizeY * y) + i, GetPixel(Surface, x, y));

    return res;
}
