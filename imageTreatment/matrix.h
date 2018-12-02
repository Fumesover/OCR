#ifndef OCR_MATRIX_H
#define OCR_MATRIX_H

#include "image.h"


/*****************************************************/
/*                   INITS                           */
/*****************************************************/
int** InitIntMatrix(int h, int w);
Pixel** InitPixelMatrix(int h, int w);
void InitArray(int *array, int h);


/*****************************************************/
/*                   FREE                            */
/*****************************************************/
void FreeMatrix(void **m, int h);


/*****************************************************/
/*                   PRINT                           */
/*****************************************************/
void PrintMatrix(int **matrix, int h, int w);
void PrintArray(int *list, int h);


/*****************************************************/
/*                   HISTOGRAMS                      */
/*****************************************************/

// Returns the histogram of all columns of the matrix
// in the line range [h1, h2]
int* MatrixWHistogram(int **matrix, int h1, int h2, int w);

// Returns the histogram of all lines of the matrix
// in the line range [w1, w2]
int* MatrixHHistogram(int **matrix, int h, int w1, int w2);

/*****************************************************/
/*                   MANIPULATIONS                   */
/*****************************************************/

// Copy content of mat1 into mat2
void Copy(int **mat1, int**mat2, int h, int w);
int** CutMatrix(int **matrix, int h1, int h2, int w1, int w2);

// Returns a squared matrix equivalent to the original
// By adding white space on the thinner sides
int **SquareMatrix(int **matrix, int h, int w);

// Remove the white borders on the side of the matrix
int **RemoveWhite(int **matrix, int *h, int *w);


/*****************************************************/
/*                   RESIZE                          */
/*****************************************************/

// Returns the matrix to the adequate format and size
int **Resize(int **matrix, int h, int w, int newsize);

// Returns the squared surface resized with size t
SDL_Surface *ResizeSurface(SDL_Surface *Surface, int s);

#endif
