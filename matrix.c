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

void InitList(int *list, int h)
{
    for (int i = 0; i < h; i++)
    {
        list[i] = 0;
    }
}

void PrintList(int *list, int h)
{
    for (int i = 0; i < h; i++)
    {
        printf("(%d)=%d\n, ", i, list[i]);
    }
}

void MatrixHHistogram(int **matrix, int *histogram, int h, int w)
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            if (matrix[i][j] == 0)
                histogram[i]++;
        }
    }
}

void MatrixWHistogram(int **matrix, int *histogram, int h1, int h2, int w)
{
    for (int i = h1; i < h2; i++)
    {
        for (int j = 0; j < w; j++)
        {
            if (matrix[i][j] == 0)
                histogram[j]++;
        }
    }
}