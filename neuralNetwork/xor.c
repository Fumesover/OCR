#include "neuralNet.h"
#include "xor.h"

#include <stdio.h>

int main(void) {
    // INIT
    int   nbInp     = 2;
    float inpVals[] = {1, 1};
    int   nbHidden  = 1;
    int   hidden[]  = {3};
    int   nbOut     = 1;
    float target[]  = {0};
    char* filepath  = "xor.nn";

    float updateRate = 1.0f;

    neuNet* n = NNinit(nbInp, nbHidden, hidden, nbOut);

    if (!n) // TODO: Better errors
        return 1;

    // RANDOMISE
    NNrand(n);

    float err;
    int itteration = 0;
    float tests[4][3] = {
        {1.0f,1.0f,0.0f},
        {0.0f,0.0f,0.0f},
        {1.0f,0.0f,1.0f},
        {0.0f,1.0f,1.0f},
    };
    int testPos = 0;
    int testLen = 4;

    do {
        err = 0.0f;
        for (testPos = 0; testPos < testLen; testPos++) {
            inpVals[0] = tests[testPos][0];
            inpVals[1] = tests[testPos][1];
            target[0]  = tests[testPos][2];

            float errthis = NNTrain(n, inpVals, target, updateRate);
            err += errthis;

            printf("  n°%d", itteration++);
            printf(" => error : %f", errthis);
            printf(" vals : %1.f xor %1.f", inpVals[0], inpVals[1]);
            printf(" => %1.f (%f)\n", tests[testPos][2], n->neuOutput[0]);
        }

        printf("n°%d : batch error : ", itteration / testLen);
        printf("%f -- update rate : %f\n", err, updateRate);
    } while (err > 0.01f);

    printf("\n========\n\n");

    printf("Saving neural network to %s\n", filepath);
    NNsave(n, filepath);
    NNfree(n);

    printf("Loading file %s\n", filepath);
    neuNet* loaded = NNload(filepath);

    for (int i = 0; i < testLen; i++) {
        inpVals[0] = tests[i][0];
        inpVals[1] = tests[i][1];

        float* result = NNGuess(loaded, inpVals);
        printf(" %1.f XOR %1.f => %f\n", inpVals[0], inpVals[1], result[0]);
    }

    NNfree(loaded);
    return 0;
}

