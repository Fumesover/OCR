#ifndef OCR_MATRIX_H
#define OCR_MATRIX_H

/* INITIALIZE */
void InitMatrix(int **matrix, int h, int w);
void InitList(int *list, int h);

/* PRINT */
void PrintMatrix(int **matrix, int h, int w);
void PrintList(int *list, int h);

/* FILL */
void MatrixHHistogram(int **matrix, int *histogram, int h, int w);
void MatrixWHistogram(int **matrix, int *histogram, int h1, int h2, int w);


#endif
