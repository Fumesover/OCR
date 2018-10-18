#include "neuralNet.h"
#include "xor.h"

#include <stdio.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

// Data object.
typedef struct
{
    // 2D floating point array of input.
    float** in;
    // 2D floating point array of target.
    float** tg;
    // Number of inputs to neural network.
    int nips;
    // Number of outputs to neural network.
    int nops;
    // Number of rows in file (number of sets for neural network).
    int rows;
}
Data;

// Returns the number of lines in a file.
static int lns(FILE* const file)
{
    int ch = EOF;
    int lines = 0;
    int pc = '\n';
    while((ch = getc(file)) != EOF)
    {
        if(ch == '\n')
            lines++;
        pc = ch;
    }
    if(pc != '\n')
        lines++;
    rewind(file);
    return lines;
}

// Reads a line from a file.
static char* readln(FILE* const file)
{
    int ch = EOF;
    int reads = 0;
    int size = 128;
    char* line = (char*) malloc((size) * sizeof(char));
    while((ch = getc(file)) != '\n' && ch != EOF)
    {
        line[reads++] = ch;
        if(reads + 1 == size)
            line = (char*) realloc((line), (size *= 2) * sizeof(char));
    }
    line[reads] = '\0';
    return line;
}

// New 2D array of floats.
static float** new2d(const int rows, const int cols)
{
    float** row = (float**) malloc((rows) * sizeof(float*));
    for(int r = 0; r < rows; r++)
        row[r] = (float*) malloc((cols) * sizeof(float));
    return row;
}

// New data object.
static Data ndata(const int nips, const int nops, const int rows)
{
    const Data data = {
        new2d(rows, nips), new2d(rows, nops), nips, nops, rows
    };
    return data;
}

// Gets one row of inputs and outputs from a string.
static void parse(const Data data, char* line, const int row)
{
    const int cols = data.nips + data.nops;
    for(int col = 0; col < cols; col++)
    {
        const float val = atof(strtok(col == 0 ? line : NULL, " "));
        if(col < data.nips)
            data.in[row][col] = val;
        else
            data.tg[row][col - data.nips] = val;
    }
}

// Frees a data object from the heap.
static void dfree(const Data d)
{
    for(int row = 0; row < d.rows; row++)
    {
        free(d.in[row]);
        free(d.tg[row]);
    }
    free(d.in);
    free(d.tg);
}

// Randomly shuffles a data object.
static void shuffle(const Data d)
{
    for(int a = 0; a < d.rows; a++)
    {
        const int b = rand() % d.rows;
        float* ot = d.tg[a];
        float* it = d.in[a];
        // Swap output.
        d.tg[a] = d.tg[b];
        d.tg[b] = ot;
        // Swap input.
        d.in[a] = d.in[b];
        d.in[b] = it;
    }
}

// Parses file from path getting all inputs and outputs for the neural network. Returns data object.
static Data build(const char* path, const int nips, const int nops)
{
    FILE* file = fopen(path, "r");
    if(file == NULL)
    {
        printf("Could not open %s\n", path);
        printf("Get it from the machine learning database: ");
        printf("wget http://archive.ics.uci.edu/ml/machine-learning-databases/semeion/semeion.data\n");
        exit(1);
    }
    const int rows = lns(file);
    Data data = ndata(nips, nops, rows);
    for(int row = 0; row < rows; row++)
    {
        char* line = readln(file);
        parse(data, line, row);
        free(line);
    }
    fclose(file);
    return data;
}

int main()
{
    int nips = 256;
    int nops = 10;
    
    float rate = 1.0f;
    int nbLayers = 1;
    int nbHidden[] = {16, 16};
    float anneal = 0.99f;
    int iterations = 128;
    
    Data data = build("semeion.data", nips, nops);
    Data data2 = build("semeion.data", nips, nops); data2 = data2;
    
    neuNet* nn = NNinit(nips, nbLayers, nbHidden, nops);
    NNrand(nn);

    for(int i = 0; i < iterations; i++)
    {
        shuffle(data);
        float error = 0.0f;
        for(int j = 0; j < data.rows; j++) // 1; j++)
        {
            float* const in = data.in[j];
            float* const tg = data.tg[j];
            error += NNTrain(nn, in, tg, rate);
        }
        printf("error %.12f :: learning rate %f\n",
            (double) error / data.rows,
            (double) rate);
        // rate *= anneal;
        anneal = anneal;
    }
    
    // NNsave(nn, "saved.tinn");
    NNfree(nn);
    
    /*
    printf("===================");
    fflush(stdout);

    neuNet* loaded = NNload("saved.tinn");
    
    printf("nbHidden[0] = %d", nn->nbHidden[0]);

    float* const in = data.in[0];
    float* const tg = data.tg[0];
    float* const pd = NNGuess(loaded, in);
    
    for (int i = 0; i < nops; i++)
        printf("%f ", tg[i]);
    printf("\n");
    
    for (int i = 0; i < nops; i++)
        printf("%f ", pd[i]);
    printf("\n");
    
    NNfree(loaded);
    
    */
    
    dfree(data);
    dfree(data2);
    return 0;
}

