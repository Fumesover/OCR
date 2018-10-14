#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

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

// Initializes all values of the matrix to 0
void InitMatrix(int **matrix, int h, int w)
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            matrix[i][j] = 0;
        }
    }
}

/*** Lists ***/

// Initializes all values of the list to 0
void InitList(int *list, int h)
{
    for (int i = 0; i < h; i++)
    {
        list[i] = 0;
    }
}

// Prints the list of the matrix into the terminal
void PrintList(int *list, int h)
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

