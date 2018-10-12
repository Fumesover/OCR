#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"


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

// Initialize all values of the matrix to 0
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