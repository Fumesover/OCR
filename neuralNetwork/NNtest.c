#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "DataSet.h"
#include "neuralNet.h"

int findMaxPos(float* arr, int len) {
    float val = arr[0];
    int   pos = 0;

    for (int i = 1; i < len; i++) {
        if (arr[i] > val) {
            pos = i;
            val = arr[i];
        }
    }

    return pos;
}

int main() {
    DataSet* ds = getFromFile("test.inp");

    shuffle(ds);

    neuNet n = NNinit(ds->nbinputs, 28, ds->nboutputs);
    NNrand(n);
    int nbItt = 1000;

    printf("%d\n", ds->nblines);

    while (nbItt--) {
        shuffle(ds);
        for (int i = 0; i < ds->nblines; i++) {
            Data* d = ds->rows[i];
            float err = NNTrain(n, d->inputs, d->output, 1.0f);
            printf("%1.9f (%d)\n", err, nbItt);
        }
    }

    for (int i = 0; i < ds->nblines; i++) {
        Data* d = ds->rows[i];
        float* out = NNGuess(n, d->inputs);
        char c = findMaxPos(out, n.nbOutput);
        printf("%c output is : %c (%d)\n", findMaxPos(d->output, ds->nboutputs),
                                           c, (int) c);
    }

    NNfree(n);
    datasetfree(ds);
    return 0;
}
