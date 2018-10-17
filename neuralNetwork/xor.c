#include "neuralNet.h"
#include "xor.h"

#include <stdio.h>

int main(void) {
    // INIT
    int   nbInp = 2;
    float inpVals[] = {1, 1};
    int   nbHidden = 1;
    int   hidden[] = {3};
    int   nbOut = 1;
    float target[] = {0};

    float updateRate = 1.0f;

    neuNet* n = NNinit(nbInp, nbHidden, hidden, nbOut);

    if (!n)
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

            printf("  n°%d => error : %f vals : %d xor %d => %d (%f) \n",
                    itteration++, errthis, (int) tests[testPos][0],
                    (int)tests[testPos][1], (int) tests[testPos][2], n->neuOutput[0]);
        }

        printf("n°%d : batch error : %f -- update rate : %f\n",
                itteration / testLen, err, updateRate);
        updateRate *= 0.9999f;
    } while (err > 0.01f);

    NNfree(n);

    return 0;
}


