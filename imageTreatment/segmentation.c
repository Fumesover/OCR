#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "segmentation.h"
#include "image.h"
#include "queue.h"
#include "matrix.h"

#define HOR_THRESH 2000
#define VER_THRESH 2000

#define SIZE 20

Queue *queue;

// Main document segmentation functions
// Makes histogram and calls line segmentation function
Queue *Segmentation(int **matrix, int h, int w)
{
    int *histo = NULL;
    Pixel **pixels;

    /*** INIT ***/
    histo = malloc(sizeof(int) * h);
    pixels = InitPixelMatrix(h, w);

    queue = malloc(sizeof(*queue));
    queue->first = NULL;

    InitArray(histo, h);
    //MatrixHHistogram(matrix, h, w);

    /*** RLSA ***/
    RLSA(matrix, h, w);

    /*** LINE SEGMENTATION ***/
    //CutInLine(matrix, histo, queue, h,  w);

    /*** FREE ALLOCATED MEMORY ***/
    free(histo);

    for (int i = 0; i < h; i++)
        free(pixels[i]);
    free(pixels);

    return queue;
}

void RLSA(int **matrix, int h, int w)
{
    int **res = InitIntMatrix(h, w);
    int **rlsah = RLSAh(matrix, h, w);
    int **rlsaw = RLSAw(matrix, h, w);


    /*** AND OPERATOR ON BOTH MATRIXES ***/
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            res[i][j] = (rlsah[i][j] && rlsaw[i][j]) ? 1 : 0;
        }
    }
    CutInBlockH(matrix, res, h, w);

    /*** FREE ALLOCATED MEMORY ***/
    //FreeMatrix((void**)rlsah, h);
    //FreeMatrix((void**)rlsaw, h);
}

int** RLSAw(int **matrix, int h, int w) {
    int nbzeros = 0;
    int** res = InitIntMatrix(h, w);

    Copy(matrix, res, h, w);

    /*** COLUMN PROCESSING ***/
    for (int j = 0; j < w; j++)
    {
        for (int i = 0; i < h; i++)
        {
            if (matrix[i][j] == 1)
            {
                if (nbzeros <= VER_THRESH)
                {
                    for (int k = i - nbzeros; k < i; k++)
                        if (k >= 0)
                            res[k][j] = 1;
                }
                nbzeros = 0;
            }
            nbzeros++;
        }
    }

    return res;
}

int** RLSAh(int **matrix, int h, int w) {
    int nbzeros = 0;
    int** res = InitIntMatrix(h, w);

    Copy(matrix, res, h, w);

    /*** ROW PROCESSING ***/
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (matrix[i][j] == 1) {
                if (nbzeros <= HOR_THRESH) {
                    for (int k = j - nbzeros; k < j; k++)
                        res[i][k] = 1;
                }
                nbzeros = 0;
            } else
                nbzeros++;
        }
    }

    return res;
}

void CutInBlockH(int** matrix, int **rlsa, int h, int w) {
    int *histo = MatrixWHistogram(rlsa, 0, h, w);
    int begin = 0, end = w-1;
    int n = 0, sum = 0;
    int av = 0, sp = 0;

    while (begin < w && histo[begin] == 0) begin++;
    while (end > 0 && histo[end] == 0) end--;

    int i = begin;
    while (i < end){
        if (histo[i] == 0){
            while (histo[i] == 0){
                sum++;
                i++;
            }
            n++;
        }
        i++;
    }
    if (n > 0)
        av = sum / n;

    i = begin;
    int w1 = begin;
    while (i <= end)
    {
        if (histo[i] == 0 || i == end) {
            int w2 = i;
            while (histo[i] == 0 || i == end) {
                sp++;
                i++;
            }
            if (sp > av || (i == end)) {
                CutInBlockW(matrix, rlsa, h, w1, w2);
                w1 = i;
            }
            sp = 0;
        }
        i++;
    }

    free(histo);
}

void CutInBlockW(int** matrix, int **rlsa, int h, int w1, int w2) {
    int *histo = MatrixHHistogram(rlsa, h, w1, w2);
    int begin = 0, end = h-1;
    int n = 0, sum = 0;
    int av = 0, sp = 0;

    while (begin < h && histo[begin] == 0) begin++;
    while (end > 0 && histo[end] == 0) end--;

    int i = begin;
    while (i < end){
        if (histo[i] == 0){
            while (histo[i] == 0){
                sum++;
                i++;
            }
            n++;
        }
        i++;
    }

    if (n > 0)
        av = sum / n;

    i = begin;
    int h1 = begin;
    while (i <= end)
    {
        if (histo[i] == 0 ||i == end) {
            int h2 = i;
            while (histo[i] == 0 || i == end) {
                sp++;
                i++;
            }
            if (sp > av || (i == end)) {
                int **m = CutMatrix(matrix, h1, h2, w1, w2);
                int nh = h2-h1, nw = w2-w1;
                CutInLine(m, MatrixHHistogram(m, nh, 0, nw), nh, nw);
                h1 = i;
            }
            sp = 0;
        }
        i++;
    }
}


// Cuts matrix in lines of characters and sends them
// to CutInChar with the corresponding histogram
void CutInLine(int **matrix, int *histogram, int h, int w)
{
    /*** INIT ***/
    int i = 0, x1, x2;
    int *histoW = NULL;
    int space = 0;
    //float average_sp = 0.0;

    Tuple *data = NewTuple();
    data->height = 1;
    data->width = 1;

    int **eol = NULL;
    eol = malloc(sizeof(int*) * 1);
    eol[0] = malloc(sizeof(int) * 1);
    eol[0][0] = 38;

    data->data = eol;

    /*** PROCESSING ALL LINES ***/
    while (i < h)
    {
        if (histogram[i] >= 1)
        {
            // Top of line
            x1 = (i) ? i-1 : 0;

            while (histogram[i] >= 1 && i < h) i++;

            i--;
            // Bottom of line
            if (i >= h - 1) x2 = i; else x2 = i + 1;

            // Draw line around the line
            for (int j = 0; j < w; j++){
                matrix[x2][j] = 2;
                matrix[x1][j] = 2;
            }

            // Clears and creates histogram for each detected line
            histoW = MatrixWHistogram(matrix, x1, x2, w);

            // Cuts line in char
            CutInChar(matrix, histoW, x1, x2, w);

            // Enqueue eol after line is treated
            Enqueue(queue, data);
            space = 0;

            i++;
        }

        else {
            i++;
            space++;
        }
    }

    /*** FREE ALLOCATED MEMORY ***/
    free(histoW);
}


// Cutes line from h1 to h2 in "matrix" in characters
// and puts them in the queue
void CutInChar(int **matrix, int *histo, int h1, int h2, int w)
{
    /*** INIT ***/
    int i = 0, space = 0, x1 = 0, x2 = 0;
    float average_sp = 0.0;

    Tuple *data = NewTuple();
    data->height = 1;
    data->width = 1;

    int **sp = NULL;
    sp = malloc(sizeof(int*) * 1);
    sp[0] = malloc(sizeof(int) * 1);
    sp[0][0] = 32;

    data->data = sp;

    // Find average space between characters
    average_sp = AverageSpace(histo, w);

    /*** PROCESSING ALL COLUMNS ***/
    while (i < w)
    {
        if (histo[i] > 0)
        {
            // If blank pixels considered as space
            // adds space in queue before next character
            if (space > average_sp)
                Enqueue(queue, data);
            space = 0;

            // Right side of character
            x1 = (i) ? i - 1 : 0;

            while (histo[i] > 0) i++;

            // Left side of character
            x2 = i;

            // Draws line around character
            for (int j = h1; j < h2; j++) {
                matrix[j][x2] = 3;
                matrix[j][x1] = 3;
            }

            // Enqueue character
            EnqueueMatrix(matrix, h1, h2, x1, x2);
        }

        else {
            i++;
            space++;
        }
    }
}

// Returns the average white spaces in the histogram
float AverageSpace(int* histogram, int t)
{
    int pos = 0, last_pos = t - 1;
    int space = 0, min_sp = 100, max_sp = 0;

    /*** FINDS AVERAGE SPACE OF LINE ***/
    while (histogram[pos] == 0) pos++;
    while (histogram[last_pos] == 0) last_pos--;

    while (pos < last_pos)
    {
        if (histogram[pos] == 0)
        {
            while (histogram[pos] == 0) {
                pos++;
                space++;
            }

            if (space < min_sp) min_sp = space;
            if (space > max_sp) max_sp = space;
        }

        else pos++;
        space = 0;
    }

    return (float)(min_sp + max_sp) / 2;
}


void EnqueueMatrix(int **matrix, int h1, int h2, int w1, int w2)
{
    int **original;
    int h = h2-h1, w = w2-w1;
    original = InitIntMatrix(h, w);

    for (int i = h1; i < h2; i++)
    {
        for (int j = w1; j < w2; j++)
            original[i-h1][j-w1] = matrix[i][j];
    }

    // Change matrix format
    int **final = Resize(original, h, w, SIZE);
    Tuple *data = NewTuple();

    data->data      = final;
    data->height    = SIZE;
    data->width     = SIZE;
    Enqueue(queue, data);
}

// Shows result of segmentation
// Returns elements of the queue in a char array 
char* ShowSegmentation()
{
    printf("y\n");
    char *s;
    Elt *curr = NULL;
    int **c;
    int t = 0;

    s = malloc(sizeof(char) * 1000);

    if (queue->first != NULL)
        curr = queue->first;

    while (curr != NULL && curr->data != NULL)
    {
        c = curr->data->data;
        if (curr->data->width > 1 && curr->data->height > 1){
            s[t] = 'c';
            t++;

            int h = curr->data->height;
            int w = curr->data->width;

            DisplayMatrix(c, h, w);

        }

        else if (c != NULL && c[0][0] == 38) {
            s[t] = '\n';
            t++;
        }
        else if (c != NULL && c[0][0] == 32) {
            s[t] = ' ';
            t++;
        }
        else
        {
            s[t] = '\n';
            t++;
        }



        curr = curr->next;
    }

    return s;
}