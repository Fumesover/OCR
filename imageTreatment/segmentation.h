#ifndef OCR_SEGMENTATION_H
#define OCR_SEGMENTATION_H

#include "queue.h"

/*************************************************************/
/*                       SEGMENTATION                        */
/*************************************************************/

// Main segmentation function
Queue *Segmentation(int **matrix, int h, int w);

// Applies RLSA algorithm on matrix
// Links close characters together
// To simplify bloc
void RLSA(int **matrix, int h, int w);

// Returns a matrix with horizontal links between close chars
int** RLSAw(int **matrix, int h, int w);

// Returns a matrix with vertical links between close chars
int** RLSAh(int **matrix, int h, int w);

// Cuts matrix in horizontal blocks considering the links in rlsa matrix
void CutInBlockH(int** matrix, int **rlsa, int h, int w);

// Cuts matrix in vertical blocks
void CutInBlockW(int** matrix, int **rlsa, int h, int w1, int w2);

// Cuts matrix in lines of characters and sends them
// to CutInChar with the corresponding histogram
void CutInLine(int **matrix, int *histogram, int h, int w);

// Cutes line from h1 to h2 in "matrix" in characters
// and puts them in the queue
void CutInChar(int **matrix, int *histo, int h1, int h2, int w);


// Enqueues the matrix between h1 / h2 and w1 / w2 in queue
void EnqueueMatrix(int **matrix, int h1, int h2, int w1, int w2);

// Returns the average white spaces in the histogram
float AverageSpace(int* histogram, int t);


// TEST FUNCTION
// Shows result of segmentation
// Returns elements of the queue in a char array
char *ShowSegmentation();

#endif
