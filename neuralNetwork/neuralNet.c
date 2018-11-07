#include "neuralNet.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <err.h>
#define _GNU_SOURCE
#include <stdio.h>
size_t getdelim(char **lineptr, size_t *n, int delim, FILE *stream);

void NNprint(float* arr, int size) {
    for(int i = 0; i < size; i++)
        printf("%f ", (double) arr[i]);
    printf("\n");
}

neuNet* NNinit(int nbInputs, int ttHidden, int nbOutput) {
    /*
    if (nbLayers < 1) {
        printf("Error : NeuralNetwork need at least one hidden layer");
        // return NULL; // < TODO : return error
    }
    */

    neuNet* nn = malloc(sizeof(neuNet));

    // Statics
    nn->nbInputs = nbInputs;
    // nn->nbHidden = malloc(nbLayers * sizeof(int));
    // for (int l = 0; l < nbLayers; l++)
    //    nn->nbHidden[l] = nbHidden[l];
    nn->nbOutput = nbOutput;
    // nn->nbLayers = nbLayers;

    /*
    n->nbHidden = n->nbNeurons + 1;
    for (int l = 0; l < nbLayers; l++)
        nn->nbHidden[l] = nbHidden[l];
    */

    // Count hidden neurons
    nn->ttHidden = ttHidden;

    // Count weights
    nn->nbWeights = ttHidden * (nbInputs + nbOutput);

    // Count biais
    nn->nbBiais = 2;

    // Init arrays
    nn->weights   = malloc(nn->nbWeights * sizeof(*nn->weights));
    nn->biais     = malloc(nn->nbBiais   * sizeof(*nn->biais));

    int sumNeurons = nn->ttHidden + nn->nbOutput;
    nn->neuHidden = malloc(sumNeurons * sizeof(*nn->neuHidden));
    nn->neuOutput = nn->neuHidden + ttHidden;

    return nn;
}

float randF() {
    return rand() / (float) RAND_MAX;
}

void NNrand(neuNet* nn) {
    srand(time(NULL));

    for (int p = 0; p < nn->nbWeights; p++)
        nn->weights[p] = randF() - 0.5f;

    for (int p = 0; p < nn->nbBiais; p++)
        nn->biais[p] = randF() - 0.5f;
}

float activation(float inp) {
    // Sigmoid :
    return 1.0f / (1.0f + expf(-inp));;
}

float primeOfAct(float act) {
    return act * (1 - act);
}

void forwardPropagation(neuNet* n, float* inp) {
    for(int i = 0; i < n->ttHidden; i++) {
        float sum = 0;
        for(int j = 0; j < n->nbInputs; j++)
            sum += inp[j] * n->weights[i * n->nbInputs + j];
        n->neuHidden[i] = activation(sum + n->biais[0]);
    }

    float* weights = n->weights + n->nbInputs * n->ttHidden;

    for(int i = 0; i < n->nbOutput; i++) {
        float sum = 0.0f;
        for(int j = 0; j < n->ttHidden; j++)
            sum += n->neuHidden[j] * weights[i * n->ttHidden + j];
        n->neuOutput[i] = activation(sum + n->biais[1]);
    }
}

void NNfree(neuNet *n) {
    free(n->neuHidden);
    // free(n->neuOutput);

    // free(n->nbNeurons);

    free(n->weights);
    free(n->biais);

    free(n);
}

void backPropagation(neuNet* n, float* inp, float* targ, float rate) {
    int offsetWeights = n->nbInputs * n->ttHidden;

    for(int i = 0; i < n->ttHidden; i++) {
        float sum = 0.0f;

        for (int j = 0; j < n->nbOutput; j++) {
            float err   = n->neuOutput[j] - targ[j];
            float prime = primeOfAct(n->neuOutput[j]);
            int   wpos  = offsetWeights + j * n->ttHidden + i;

            sum += err * prime * n->weights[wpos];

            n->weights[wpos] -= err * prime * n->neuHidden[i] * rate;
        }

        for(int j = 0; j < n->nbInputs; j++) {
            float prime = primeOfAct(n->neuHidden[i]);
            n->weights[i * n->nbInputs + j] -= rate * sum * prime * inp[j];
        }
    }
}

float error(float a, float b) {
    // return (a - b) * (a - b);
    return 0.5 * (a - b) * (a - b);
}

float NNerror(neuNet *n, float* target) {
    float sum = 0.0f;
    for (int p = 0; p < n->nbOutput; p++)
        sum += error(n->neuOutput[p], target[p]);
    return sum;
}

float NNTrain(neuNet* n, float* inp, float* targ, float update) {
    forwardPropagation(n, inp);
    backPropagation(n, inp, targ, update);
    return NNerror(n, targ);
}

float* NNGuess(neuNet* n, float* inp) {
    forwardPropagation(n, inp);
    return n->neuOutput;
}

void NNsave(neuNet* n, char* filename){
    FILE* fPointer = fopen(filename,"w");
    
    if (!fPointer)
        errx(1, "Invalid path %s", filename);

    fprintf(fPointer,"%d\n",n->nbInputs);
    fprintf(fPointer,"%d\n",n->nbOutput);
    // fprintf(fPointer,"%d\n",n->nbLayers);
    fprintf(fPointer,"%d\n", 1);

    /* for(int i = 0; i < n->nbLayers; i++)
        fprintf(fPointer,"%d ",n->nbHidden[i]);
    fprintf(fPointer,"\n"); */
    fprintf(fPointer, "%d \n", n->ttHidden);

    for(int i = 0; i < n->nbWeights; i++)
        fprintf(fPointer,"%f ",n->weights[i]);
    fprintf(fPointer,"\n");

    for(int i = 0; i < n->nbBiais; i++)
        fprintf(fPointer,"%f ",n->biais[i]);

    fprintf(fPointer,"\n");
    fclose(fPointer);
}

neuNet* NNload(char* filename){
    FILE* fp = fopen(filename,"r");
    
    if (!fp)
        return (neuNet*) NULL;
    
    size_t len = 0;
    char* line = NULL;
    size_t read;

    int nbInputs = 0;
    if ((read = getdelim(&line, &len, '\n', fp)) != (size_t) -1)
        nbInputs = atoi(line);

    int nbOutput = 0;
    if ((read = getdelim(&line, &len, '\n', fp)) != (size_t) -1)
        nbOutput = atoi(line);

    int nbLayers = 0;
    if ((read = getdelim(&line, &len, '\n', fp)) != (size_t) -1)
        nbLayers = atoi(line);

    int* nbHidden = malloc(nbLayers * sizeof(int));
    for (int i = 0; i < nbLayers; i++)
        if ((read = getdelim(&line, &len, ' ', fp)) != (size_t) -1)
            nbHidden[i] = atoi(line);
    read = getdelim(&line, &len, '\n', fp); // remove last space

    neuNet* n = NNinit(nbInputs, nbHidden[0], nbOutput);

    free(nbHidden);

    for (int i = 0; i < n->nbWeights; i++)
        if ((read = getdelim(&line, &len, ' ', fp)) != (size_t) -1)
            n->weights[i] = atof(line);
    read = getdelim(&line, &len, '\n', fp); // remove last space

    for (int i = 0; i < n->nbBiais; i++)
        if ((read = getdelim(&line, &len, ' ', fp)) != (size_t) -1)
            n->biais[i] = atof(line);

    free(line);
    fclose(fp);

    return n;
}

