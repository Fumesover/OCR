#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "segmentation.h"
#include "image.h"
#include "queue.h"
#include "matrix.h"

#define HOR_THRESH 150
#define VER_THRESH 150

// Main document segmentation functions
// Makes histogram and calls line segmentation function
Queue *Segmentation(int **matrix, int h, int w)
{
    int *histo = NULL;
    Pixel **pixels;

    /*** INIT ***/
    histo = malloc(sizeof(int) * h);
    pixels = InitPixelMatrix(h, w);

    Queue *queue = NULL;
    queue = malloc(sizeof(*queue));
    queue->first = NULL;

    InitArray(histo, h);
    MatrixHHistogram(matrix, histo, h, w);

    /*** RLSA ***/
    /*BinToPixels(RLSA(matrix, h, w), pixels, h, w);
    DisplayImage(MatrixToSurface(pixels, h, w));*/

    /*** LINE SEGMENTATION ***/
    CutInLine(matrix, histo, queue, h,  w);

    // TEST: displays result
    /*ShowSegmentation(queue);

    BinToPixels(matrix, pixels, h, w);
    DisplayImage(MatrixToSurface(pixels, h, w));*/

    /*** FREE ALLOCATED MEMORY ***/
    free(histo);

    for (int i = 0; i < h; i++)
        free(pixels[i]);
    free(pixels);

    return queue;
}


int** RLSA(int **matrix, int h, int w) {
    int nbzeros = 0;
    int** resH;
    int** resW;
    int** res;

    resH = InitIntMatrix(h, w);
    resW = InitIntMatrix(h, w);
    res = InitIntMatrix(h, w);

    Copy(matrix, resH);
    Copy(matrix, resW);

    /*** ROW PROCESSING ***/
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            if (matrix[i][j] == 1)
            {
                    if (nbzeros <= HOR_THRESH)
                    {
                        for (int k = j - nbzeros + 1; k < j-1; k++)
                            resH[i][k] = 4;
                    }
                    nbzeros = 0;
            }
            else
                nbzeros++;
        }
    }

    nbzeros = 0;

    /*** COLUMN PROCESSING ***/
    for (int j = 0; j < w; j++)
    {
        for (int i = 0; i < h; i++)
        {
            if (matrix[i][j] == 1)
            {
                    if (nbzeros <= VER_THRESH)
                    {
                        for (int k = i - nbzeros + 1; k < i-1; k++)
                            if (k >= 0)
                                resW[k][j] = 4;
                    }
                    nbzeros = 0;
            }
            nbzeros++;
        }
    }

    /*** AND OPERATOR ON BOTH MATRIXES ***/
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            res[i][j] = (resW[i][j] && resH[i][j]) ? 4 : 0;
        }
    }

    /*** FREE ALLOCATED MEMORY ***/
    for (int i = 0; i < h; i++) {
        free(resH[i]);
        free(resW[i]);
    }

    return res;
}


// Cuts matrix in lines of characters and sends thems
// to CutInChar with the corresponding histogram
void CutInLine(int **matrix, int *histogram, Queue *queue, int h, int w)
{
    /*** INIT ***/
    int i = 0, x1, x2;
    int *histoW = NULL;
    int space = 0;
    //float average_sp = 0.0;

    Tuple *data = NewTuple();
    data->height = 1;
    data->width = 1;

    histoW = malloc(sizeof(int) * w);

    int **eol = NULL;
    eol = malloc(sizeof(int*) * 1);
    eol[0] = malloc(sizeof(int) * 1);
    eol[0][0] = 10;

    data->data = eol;

    // Find average space between lines
    //average_sp = AverageSpace(histogram, h);

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
            InitArray(histoW, w);
            MatrixWHistogram(matrix, histoW, x1, x2, w);

            // Cuts line in char
            CutInChar(matrix, histoW, queue, x1, x2, w);

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

    free(eol[0]);
    free(eol);
}


// Cutes line from h1 to h2 in "matrix" in characters
// and puts them in the queue
void CutInChar(int **matrix, int *histo, Queue *queue, int h1, int h2, int w)
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
            EnqueueMatrix(matrix, queue, h1, h2, x1, x2);
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


void EnqueueMatrix(int **matrix, Queue *queue, int h1, int h2, int w1, int w2)
{
    int **new;
    new = InitIntMatrix(h2-h1, w2-w1);

    for (int i = h1; i < h2; i++)
    {
        for (int j = w1; j < w2; j++)
            new[i-h1][j-w1] = matrix[i][j];
    }

    Tuple *data = NewTuple();

    data->data = new;
    data->height = h2-h1;
    data->width = w2-w1;
    Enqueue(queue, data);
}

// Shows result of segmentation
// Returns elements of the queue in a file
char* ShowSegmentation(Queue *queue)
{
    char *s;
    Elt *curr = NULL;
    int **c;
    //Pixel **m;
    //FILE* file = fopen("out.treated","w");
    int t = 0;

    s = malloc(sizeof(char) * 1000);

    if (queue->first != NULL)
        curr = queue->first;

    while (curr != NULL && curr->data != NULL)
    {
        c = curr->data->data;

        if (c[0][0] == 10) {
            s[t] = '\n';
            t++;
        }
        else if (c[0][0] == 32) {
            s[t] = ' ';
            t++;
        }
        else if (curr->data->width > 1 && curr->data->height > 1){
            s[t] = 'c';
            t++;

            //int h = curr->data->height;
            //int w = curr->data->width;

            //int **rm = RemoveWhite(c, &h, &w);
            //int **square = SquareMatrix(rm, h, w);

            //if (h > w) t = h; else t = w;

            //m = InitPixelMatrix(t, t);

            //BinToPixels(square, m, t, t);
            //DisplayImage(MatrixToSurface(m, t, t));

            //free(m);
        }
        else
        {
            s[t] = '\n';
            t++;
        }

        curr = curr->next;
    }

    // Closing file
    //fclose(file);
    return s;
}
