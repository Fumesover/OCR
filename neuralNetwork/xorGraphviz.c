#include "neuralNet.h"
#include "NNgraphviz.h"
#include <stdio.h>
#include <string.h>

int main(void) { // int argc, char** argv) {
    // INIT
    int   nbInp = 2;
    float inpVals[] = {1, 1};
    int   nbHidden = 1;
    int   hidden[] = {3};
    int   nbOut = 1;
    float target[] = {0};

    float updateRate = 0.4f;

    neuNet* n = NNinit(nbInp, nbHidden, hidden, nbOut);
    
    if (!n)
        return 1;

    // RANDOMISE
    NNrand(n);
    
    float err;
    float tests[4][3] = {
        {1.0f,1.0f,0.0f},
        {0.0f,0.0f,0.0f},
        {1.0f,0.0f,1.0f},
        {0.0f,1.0f,1.0f},
    };
    int testPos = 0;
    int testLen = 4;
    int itteration = 0;

    char* outputs[4] = {
        "xor-1-",
        "xor-2-",
        "xor-3-",
        "xor-4-" 
    };

    do {
        err = 0.0f;
        for (testPos = 0; testPos < testLen; testPos++) {
            inpVals[0] = tests[testPos][0];
            inpVals[1] = tests[testPos][1];
            target[0]  = tests[testPos][2];
            
            float errthis = NNTrain(n, inpVals, target, updateRate);
            err += errthis;
            
            char path[20];
            char num[15];
            strcpy(path, outputs[testPos]);
            sprintf(num, "%09d", itteration);
            strcat(path, num);
            FILE* file = fopen(path, "w");
            printf("%s\n", path);
            NNtoGraphviz(file, n, inpVals);
            fclose(file);
        }
        
        itteration++;
        updateRate = 0.99f;
    } while (err > 0.01f);
    
    NNfree(n);

    return 0;

}
