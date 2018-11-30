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
void MatrixHHistogram(int **matrix, int *histogram, int h, int w);
void MatrixWHistogram(int **matrix, int *histogram, int h1, int h2, int w);

void Copy(int **mat1, int**mat2, int h, int w);
int **SquareMatrix(int **matrix, int h, int w);
int **Resize(int **matrix, int h, int w, int newsize);
SDL_Surface *ResizeSurface(SDL_Surface *Surface, Uint16 t);
int **RemoveWhite(int **matrix, int *h, int *w);

#endif
