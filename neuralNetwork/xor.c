#include "neuralNet.h"
#include "DataSet.h"

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    DataSet* ds = getFromFile("xor.data");
    char* filepath = "xor.nn";
    shuffle(ds);

    float updateRate = 1.0f;

    neuNet n = NNinit(2, 3, 1);
    NNrand(n);

    float err;
    int nbItt = 100;
    int itteration = 0;

    do {
        shuffle(ds);
        err = 0.0f;

        for (int testPos = 0; testPos < ds->nblines; testPos++) {
            Data* d = ds->rows[testPos];
            float errthis = NNTrain(n, d->inputs, d->output, updateRate);
            err += errthis;

            itteration++;
           /* printf("  n°%d", itteration);
            printf(" => error : %f", errthis);
            printf(" vals : %1.f xor %1.f", d->inputs[0], d->inputs[1]);
            printf(" => %1.f (%f)\n", d->output[0], n->neuOutput[0]); */
        }

        printf("n°%d : batch error : ", itteration / ds->nblines);
        printf("%f -- update rate : %f\n", err, updateRate);
    } while (--nbItt);

    printf("\n========\n\n");

    printf("Saving neural network to %s\n", filepath);
    NNsave(n, filepath);
    NNfree(n);

    printf("Loading file %s\n", filepath);
    neuNet loaded = NNload(filepath);

    float data[4][3] = {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
    };

    for (int i = 0; i < 4; i++) {
        float* out = NNGuess(loaded, data[i]);
        printf(" %1.f XOR %1.f => %f\n", data[i][0], data[i][1], out[0]);
    }

    NNfree(loaded);

    datasetfree(ds);
    return 0;
}

