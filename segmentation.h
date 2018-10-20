#ifndef OCR_SEGMENTATION_H
#define OCR_SEGMENTATION_H

#include "queue.h"

/*SEGMENTATION */
void Segmentation(int **matrix, int h, int w);
int** RLSA(int **matrix, int h, int w);
void CutInLine(int **matrix, int *histogram, Queue *queue, int h, int w);
void CutInChar(int **matrix, int *histogram, Queue *queue, int h1, int h2, int w);
void ShowSegmentation(Queue *queue);

void EnqueueMatrix(int **matrix, Queue *queue, int h1, int h2, int w1, int w2);
float AverageSpace(int* histogram, int t);

#endif
