#ifndef OCR_MATRIX_H
#define OCR_MATRIX_H

void PrintMatrix(int **matrix, int h, int w);
void InitMatrix(int **matrix, int h, int w);
void InitList(int *list, int h);
void PrintList(int *list, int h);
void MatrixHHistogram(int **matrix, int *histogram, int h, int w);
void MatrixWHistogram(int **matrix, int *histogram, int h, int w);

#endif
