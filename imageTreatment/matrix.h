#ifndef OCR_MATRIX_H
#define OCR_MATRIX_H

#include "image.h"

/* INITIALIZE */
int** InitIntMatrix(int h, int w);
Pixel** InitPixelMatrix(int h, int w);
void InitArray(int *array, int h);

/* FREE */
void FreeMatrix(void **m, int h);

/* PRINT */
void PrintMatrix(int **matrix, int h, int w);
void PrintArray(int *list, int h);

/* FILL HISTOGRAM */
int* MatrixWHistogram(int **matrix, int h1, int h2, int w);
int* MatrixW1Histogram(int **matrix, int w1, int w2, int h);
int* MatrixHHistogram(int **matrix, int h, int w1, int w2);

void Copy(int **mat1, int**mat2, int h, int w);
int** CutMatrix(int **matrix, int h1, int h2, int w1, int w2);
int **SquareMatrix(int **matrix, int h, int w);
int **Resize(int **matrix, int h, int w, int newsize);
SDL_Surface *ResizeSurface(SDL_Surface *Surface, size_t s);
int **RemoveWhite(int **matrix, int *h, int *w);

#endif
