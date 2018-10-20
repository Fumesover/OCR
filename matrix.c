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
            printf("(%d,%d)=%d, ", i, j, matrix[i][j]);
        }
        printf("\n");
    }
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
void Copy(int **mat1, int**mat2)
{
    int w = sizeof(mat1) / 8;
    int h = sizeof(mat1[0]) / 8;
    for (int i = 0; i < h; i++)
    {
         for (int j = 0; j < w; j++)
            mat2[i][j] = mat1[i][j];
    }
}

int **RemoveWhite(int **matrix, int *h, int *w)
{
    /*** INIT ***/
    int* histoH = malloc(sizeof(int) * *h);
    int* histoW = malloc(sizeof(int) * *w);
    int** res;
    int resH = 0, resW = 0 ;
    int y = 0, x = 0, rx = 0, ry =0;

    InitArray(histoH, *h);
    InitArray(histoW, *w);

    MatrixHHistogram(matrix, histoH, *h, *w);
    MatrixWHistogram(matrix, histoW, 0, *h, *w);

    // Finds size of result matrix -> black pixels only
    for (int i = 0; i < *h; i++) {
        if (histoH[i] != 0)
            resH++;
    }

    for (int i = 0; i < *w; i++) {
        if (histoW[i] != 0)
            resW++;
    }

    res = InitIntMatrix(resH, resW);

    while (y < *h)
    {
        if (histoH[y] > 0) {
            while (histoH[y] > 0) {
                while (x < *w)
                {
                    if (histoW[x] > 0) {
                        while (histoW[x] > 0) {
                            res[ry][rx] = matrix[y][x];
                            x++;
                            rx++;
                        }
                    }
                    else x++;
                }
                rx = 0;
                x = 0;
                ry++;
                y++;
            }
        }
        else y++;
    }
    *h = resH;
    *w = resW;
    return res;
}


// Returns a squared matrix equivalent to the original
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

// Resize the matrix
int **ResizeMatrix(int **matrix, int t)
{
    double xscale = (double) SIZE / t;
    double yscale = (double) SIZE / t;
    double threshold = 0.5 / (xscale * yscale);
    double yend = 0.0;

    int **resize = malloc(sizeof(int*) * t);
    for (int i = 0; i < t; i++)
        resize[i] = malloc(sizeof(int) * t);

    for (int f = 0; f < SIZE; f++) // y on output
    {
        double ystart = yend;
        yend = (f + 1) / yscale;
        if (yend >= t) yend = t - 0.000001;
        double xend = 0.0;
        for (int g = 0; g < SIZE; g++) // x on output
        {
            double xstart = xend;
            xend = (g + 1) / xscale;
            if (xend >= t) xend = t - 0.000001;
            double sum = 0.0;
            for (int y = (int)ystart; y <= (int)yend; ++y)
            {
                double yportion = 1.0;
                if (y == (int)ystart) yportion -= ystart - y;
                if (y == (int)yend) yportion -= y+1 - yend;
                for (int x = (int)xstart; x <= (int)xend; ++x)
                {
                    double xportion = 1.0;
                    if (x == (int)xstart) xportion -= xstart - x;
                    if (x == (int)xend) xportion -= x+1 - xend;
                    sum += resize[y][x] * yportion * xportion;
                }
            }
            matrix[f][g] = (sum > threshold) ? 1 : 0;
        }
    }

    return resize;
}
