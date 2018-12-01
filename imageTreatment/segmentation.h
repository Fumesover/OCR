#ifndef OCR_SEGMENTATION_H
#define OCR_SEGMENTATION_H

#include "queue.h"

/*SEGMENTATION */
Queue *Segmentation(int **matrix, int h, int w);
int** RLSAh(int **matrix, int h, int w);
int** RLSAw(int **matrix, int h, int w);
void RLSA(int **matrix, int h, int w);
void CutInBlockH(int** matrix, int h, int w);
void CutInBlockW(int** matrix, int h, int w, int w1, int w2);
void CutInLine(int **matrix, int *histogram, int h, int w);
void CutInChar(int **matrix, int *histo, int h1, int h2, int w);
char *ShowSegmentation(Queue *queue);

void EnqueueMatrix(int **matrix, Queue *queue, int h1, int h2, int w1, int w2);
float AverageSpace(int* histogram, int t);

#endif
