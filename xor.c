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

    float updateRate = 0.1;

    neuNet* n = NNinit(nbInp, nbHidden, hidden, nbOut);
    
    if (!n)
        return 1;

    // RANDOMISE
    neuNetRandom(n);
    
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

            forwardPropagation(n, inpVals);
            backPropagation(n, inpVals, target, updateRate);

            err += NNerror(n, target);
            printf("  n°%d => error : %f vals : %d xor %d => %d \n", 
                    itteration++, NNerror(n, target), (int) tests[testPos][0], 
                    (int)tests[testPos][1], (int) tests[testPos][2]);
        }

        printf("n°%d : batch error : %f\n", itteration / testLen, err);
 
    } while (err > 0.1f);
    
    freeNeuNet(n);

    return 0;
}


