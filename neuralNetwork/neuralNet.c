#include "neuralNet.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>

#define _GNU_SOURCE
#include <stdio.h>
size_t getdelim(char **lineptr, size_t *n, int delim, FILE *stream);

int NNmain(void) {
    // INIT
    int   nbInp = 2;
    int   nbHidden = 2;
    int   hidden[] = {3, 4};
    int   nbOut = 1;

    neuNet* n = NNinit(nbInp, nbHidden, hidden, nbOut);

    // RANDOMISE
    NNrand(n);

    // ici n est le reseau
    NNsave(n, "test.txt");

    NNfree(n);

    n = NNload("test.txt");

    NNfree(n);

    return 0;
}

neuNet* NNinit(int nbInputs, int nbLayers, int* nbHidden, int nbOutput){

    if (nbLayers < 1) {
        printf("Error : NeuralNetwork need at least one hidden layer");
        // return NULL; // < TODO : return error
    }

    neuNet* nn = malloc(sizeof(neuNet));

    // Statics
    nn->nbInputs = nbInputs;
    nn->nbHidden = malloc(nbLayers * sizeof(int));
    for (int l = 0; l < nbLayers; l++)
        nn->nbHidden[l] = nbHidden[l];
    nn->nbOutput = nbOutput;
    nn->nbLayers = nbLayers;

    // Count hidden neurons
    nn->ttHidden = 0;
    for (int p = 0; p < nbLayers; p++)
        nn->ttHidden += nbHidden[p];

    // Count weights
    nn->nbWeights = nbInputs * nbHidden[0];
    for(int p = 0; p < nbLayers - 1; p++)
        nn->nbWeights += nbHidden[p] * nbHidden[p + 1];
    nn->nbWeights += nbHidden[nbLayers - 1] * nbOutput;

    // Count biais
    nn->nbBiais = nbOutput + nn->ttHidden;

    // Init arrays
    nn->weights   = malloc(nn->nbWeights * sizeof(*nn->weights));
    nn->biais     = malloc(nn->nbBiais   * sizeof(*nn->biais));
    nn->neuHidden = malloc(nn->ttHidden  * sizeof(*nn->neuHidden));
    nn->neuOutput = malloc(nn->nbOutput  * sizeof(*nn->neuOutput));

    return nn;
}

float randF() {
    return rand() / (float) RAND_MAX;
}

void NNrand(neuNet* nn) {
    srand(time(NULL));

    for (int p = 0; p < nn->nbWeights; p++)
        nn->weights[p] = randF();

    for (int p = 0; p < nn->nbBiais; p++)
        nn->biais[p] = randF();
}

float activation(float inp) {
    // Sigmoid :
    return 1.0f / (1.0f + expf(-inp));;
}

float primeOfAct(float act) {
    return act * (1 - act);
}

void oneLayerPropagation(float* previous, const int nbPrev, float* weights,
                         float* biais, float* destination, const int nbDest) {

    for (int d = 0; d < nbDest; d++) {

        float sum = 0.0f;
        for (int p = 0; p < nbPrev; p++)
            sum += previous[p] * weights[p * nbDest + d];

        destination[d] = activation(sum + biais[d]);
    }
}

void forwardPropagation(neuNet* n, float* inp) {
    oneLayerPropagation(inp, n->nbInputs, n->weights,
                        n->biais, n->neuHidden, n->nbHidden[0]);

    // Hidden -> Hidden
    float* biais   = n->biais + n->nbHidden[0];
    float* weights = n->weights + n->nbInputs * n->nbHidden[0];
    float* neuHidden = n->neuHidden;

    for (int layer = 0; layer < n->nbLayers - 1; layer++) {
        oneLayerPropagation(neuHidden, n->nbHidden[layer], weights, biais,
                     neuHidden + n->nbHidden[layer], n->nbHidden[layer + 1]);

        biais += n->nbHidden[layer];
        weights   += n->nbHidden[layer] * n->nbHidden[layer + 1];
        neuHidden += n->nbHidden[layer];
    }

    // Hidden -> Output
    oneLayerPropagation(neuHidden, n->nbHidden[n->nbLayers - 1],
                        weights, biais, n->neuOutput, n->nbOutput);
}

void NNfree(neuNet *n) {
    free(n->neuHidden);
    free(n->neuOutput);

    free(n->nbHidden);

    free(n->weights);
    free(n->biais);

    free(n);
}

void backPropagation(neuNet* n, float* inp, float* targ, float rate) {
    // Compute Costs

    // Outputs errors
    float *costOutp = malloc(n->nbOutput * sizeof(float));
    for (int o = 0; o < n->nbOutput; o++)
        costOutp[o] = 2 * (targ[o] - n->neuOutput[o]);

    float *costH = malloc(n->ttHidden * sizeof(float));

    // Hidden <-> Ouput
    int nbH  = n->nbHidden[n->nbLayers - 1];
    int wPos = n->nbWeights - n->nbHidden[n->nbLayers - 1] * n->nbOutput;
    for (int posH = n->ttHidden - nbH; posH < n->ttHidden; posH++) {
        float sum = 0.0f;

        for (int o = 0; o < n->nbOutput; o++)
        {
            sum += costOutp[o] * n->weights[wPos] * primeOfAct(n->neuOutput[o]);
            wPos++;
        }

        costH[posH] = sum;
    }

    // Hidden <-> Hidden
    wPos -= n->nbHidden[n->nbLayers - 1] * n->nbOutput;
    int posH = n->ttHidden - nbH;

    for (int layer = n->nbLayers - 2; layer >= 0; layer--) {
        posH -= n->nbHidden[layer];
        wPos -= n->nbHidden[layer] * n->nbHidden[layer + 1];

        for (int pH = posH; pH < posH + n->nbHidden[layer]; pH++) {
            float sum = 0.0f;

            int neuNext = posH + n->nbHidden[layer];
            int neuEnd  = posH + n->nbHidden[layer] + n->nbHidden[layer + 1];
            for (; neuNext < neuEnd; neuNext++) {
                sum += costH[neuNext] * n->weights[wPos++]
                                      * primeOfAct(n->neuHidden[neuNext]);
            }

            costH[pH] = sum;
        }

        wPos -= n->nbHidden[layer] * n->nbHidden[layer + 1];
    }

    // Update weights

    // Last Hidden -> Output
    wPos = n->nbWeights - n->nbHidden[n->nbLayers - 1] * n->nbOutput;
    int pos = n->ttHidden - n->nbHidden[n->nbLayers - 1];
    for (; pos < n->ttHidden; pos++) {
        for (int posD = 0; posD < n->nbOutput; posD++) {
            float toUp = costOutp[posD] * primeOfAct(n->neuOutput[posD])
                                          * n->neuHidden[pos];
            n->weights[wPos++] += toUp * rate;
        }
    }

    // Hidden to Hidden
    posH = n->ttHidden - nbH;
    wPos -= n->nbHidden[n->nbLayers - 1] * n->nbOutput;

    for (int layer = n->nbLayers - 2; layer >= 0; layer--) {
        posH -= n->nbHidden[layer];
        wPos -= n->nbHidden[layer] * n->nbHidden[layer + 1];

        int pEnd = posH + n->nbHidden[layer];
        for (int pSource = posH; pSource < pEnd; pSource++) {
            int pDest = posH + n->nbHidden[layer];
            int pBlock = posH + n->nbHidden[layer] + n->nbHidden[layer + 1];

            for (; pDest < pBlock; pDest++) {
                float toUp = costH[pDest] * primeOfAct(n->neuHidden[pDest])
                                          * n->neuHidden[pSource];
                n->weights[wPos++] += toUp * rate;
            }
        }

        wPos -= n->nbHidden[layer] * n->nbHidden[layer + 1];
    }

    // Input to Hidden
    wPos = 0;
    for (int pos = 0; pos < n->nbInputs; pos++) {
        for (int posD = 0; posD < n->nbHidden[0]; posD++) {
            float toUp = costH[posD] * primeOfAct(n->neuHidden[posD]) *inp[pos];
            n->weights[wPos++] += toUp * rate;
        }
    }

    // Update biais
    // Output Layer
    for (int pos = 0; pos < n->nbOutput; pos++) {
        float toUp = costOutp[pos] * primeOfAct(n->neuOutput[pos]);
        n->biais[n->ttHidden + pos] += toUp * rate;
    }

    // Hidden Layers
    for (posH = 0; posH < n->ttHidden; posH++) {
        float toUp = costH[posH] * primeOfAct(n->neuHidden[posH]);
        n->biais[posH] += toUp * rate;
    }

    free(costH);
    free(costOutp);
}

float error(float a, float b) {
    // return (a - b) * (a - b);
    return 0.5 * (a - b) * (a - b);
}

float NNerror(neuNet *n, float* target) {
    float sum = 0.0f;
    for (int p = 0; p < n->nbOutput; p++)
        sum += error(n->neuOutput[p], target[p]);
    return sum / (float) n->nbOutput;
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

    fprintf(fPointer,"%d\n",n->nbInputs);
    fprintf(fPointer,"%d\n",n->nbOutput);
    fprintf(fPointer,"%d\n",n->nbLayers);

    for(int i = 0; i < n->nbLayers; i++)
        fprintf(fPointer,"%d ",n->nbHidden[i]);
    fprintf(fPointer,"\n");

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

    neuNet* n = NNinit(nbInputs, nbLayers, nbHidden, nbOutput);

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
