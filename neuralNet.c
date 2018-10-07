#include "neuralNet.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int NNmain(void){
    
    // INIT
    int   nbInp = 2;
    float inpVals[] = {0.5, 0.7};
    int   nbHidden = 1;
    int   hidden[] = {2};
    int   nbOut = 2;
    float target[] = {0.5, 0.7};

    float updateRate = 0.04;

    neuNet n = NNinit(nbInp, nbHidden, hidden, nbOut);

    printf("nbInputs : %d\n", n.nbInputs);
    printf("nbOutput : %d\n", n.nbOutput);
    printf("ttHidden : %d\n", n.ttHidden);
    printf("nbLayers : %d\n", n.nbLayers);

    for(int i = 0; i < 2; i++){
        printf("layer %d : nbHidden : %d\n", i, n.nbHidden[i]);
    }
    
    printf("nbWeights : %d\n", n.nbWeights);
    printf("nbBiais : %d\n", n.nbBiais);
    

    // RANDOMISE
    neuNetRandom(n);
    

    // Propagation
    //float vals[] = {0.5, 0.7};
    forwardPropagation(n, inpVals);
    
    // BackPropagation
    backPropagation(n, inpVals, target, updateRate);


    freeNeuNet(n);

    return 0;
}

/* Nb hidden = 
 *              < 2 * nbInputs
 *              nbInput < xxxx < nbOuput (si beaucoup de nodes)
 *              2 / 3 * nbInput + nbOutput (maybe)
 *              (faire un mix de tout ca !)
*/

neuNet NNinit(const int nbInputs, const int nbLayers, 
                int* nbHidden, const int nbOutput){
    
    if (nbLayers < 1) {
        printf("Error : NeuralNetwork need at least one hidden layer");
        // return NULL; // < TODO : gerer fail
    }
    
    neuNet nn;
    
    // Statiques
    nn.nbInputs = nbInputs;
    nn.nbHidden = nbHidden;
    nn.nbOutput = nbOutput;
    nn.nbLayers = nbLayers;
    
    // Calcul nb neurones caches
    nn.ttHidden = 0;
    for (int p = 0; p < nbLayers; p++) {
        nn.ttHidden += nbHidden[p];
    }

    // Calcul nb poids
    int p = 0;
    nn.nbWeights = nbInputs * nbHidden[p];
    for(; p < nbLayers - 1; p++) {
        nn.nbWeights += nbHidden[p] * nbHidden[p + 1];
    }
    nn.nbWeights += nbHidden[p] * nbOutput;

    // Calcul nb biais
    nn.nbBiais = nbOutput + nn.ttHidden;
    
    // Initialisations des tableaux
    nn.weights = malloc(nn.nbWeights * sizeof(*nn.weights));
    nn.biais = malloc(nn.nbBiais * sizeof(*nn.biais));
    
    // Initialisation des neurones
    nn.neuInputs = malloc(nn.nbInputs * sizeof(*nn.neuInputs));
    nn.neuHidden = malloc(nn.ttHidden * sizeof(*nn.neuHidden));
    nn.neuOutput = malloc(nn.nbOutput * sizeof(*nn.neuOutput));

    return nn; 
}

void neuNetRandom(neuNet nn) {
    srand(time(NULL));
    
    printf("Weights :\n");
    for (int p = 0; p < nn.nbWeights; p++) {
        nn.weights[p] = randF();
        printf("%d -> %f\n",p, nn.weights[p]);
    }
    
    printf("Biais :\n");
    for (int p = 0; p < nn.nbBiais; p++) {
        nn.biais[p] = randF();
        printf("%d -> %f\n", p, nn.biais[p]);
    }
}

float randF() {
    return rand() / (float) RAND_MAX;
}

void forwardPropagation(neuNet n, float* inp) {
    
    printf("fpropagation input to lvl 1\n");

    oneLayerPropagation(inp, 0, n.nbInputs, 
                        n.weights, 0, 
                        n.biais, 0,
                        n.neuHidden, 0, n.nbHidden[0]);

    int offsetBiais   = 0;
    int offsetWeights = n.nbInputs * n.nbHidden[0];
    int offsetHidden  = 0;
    
    printf("n.nbLayers : %d\n", n.nbLayers);

    for (int layer = 0; layer < n.nbLayers -1; layer++) {
        printf("fpropagation lvl %d to lvl %d\n", layer, layer + 1);

        oneLayerPropagation(n.neuHidden, offsetHidden, n.nbHidden[layer] + offsetHidden,
                            n.weights, offsetWeights,
                            n.biais,   offsetBiais + n.nbHidden[layer], 
                            n.neuHidden, offsetHidden + n.nbHidden[layer], n.nbHidden[layer + 1] + (offsetHidden + n.nbHidden[layer]));
        //offset

        offsetBiais   += n.nbHidden[layer];
        offsetWeights += n.nbHidden[layer] * n.nbHidden[layer + 1];
        offsetHidden  += n.nbHidden[layer];
    }
    
    printf("fpropagation lvl %d to output\n", n.nbLayers - 1);

    oneLayerPropagation(n.neuHidden, offsetHidden, 
                                      n.nbHidden[n.nbLayers - 1] + offsetHidden,
                        n.weights, offsetWeights,
                        n.biais,   offsetBiais + n.nbOutput, 
                        n.neuOutput, 0, n.nbOutput);

    
}

void oneLayerPropagation(float* previous, const int pStart, const int pEnd,
                         float* weights,  const int wStart, //const int wEnd,
                         float* biais,    const int bStart,
                         float* destination, const int dStart, const int dEnd) {
    int nbDest = dEnd - dStart;
    int nbPrev = pEnd - pStart;

    for (int d = 0; d < nbDest; d++) {
        float sum = 0.0f;

        for (int p = 0; p < nbPrev; p++) {
            
            sum += previous[p + pStart] * weights[p * nbDest + d + wStart];
            printf("  wpos : %d | wval : %f | ppos : %d | pval : %f | sum : %f\n",
                  p * nbDest + d + wStart, weights[p * nbDest + d + wStart], 
                  p + pStart, previous[p + pStart], sum);
        }
        
        printf(" %d : sum = %f, biais = %f, neuron value = %f\n", 
                dStart +d, sum, biais[bStart + d], 
                              activation(sum + biais[bStart + d]));
        
        destination[dStart + d] = activation(sum + biais[bStart + d]);  
    }
}

float activation(float inp) { 
    // Sigmoid :
    return 1.0f / (1.0f + expf(-inp));;
}

float activationPrime(float inp) {
    float act = activation(inp);
    return act * (1 - act);
}

float primeOfActivation(float act) {
    return act * (1 - act);
}

void freeNeuNet(neuNet n) {
    free(n.neuInputs);
    free(n.neuHidden);
    free(n.neuOutput);
    
    free(n.weights);
    free(n.biais);
}

void backPropagation(neuNet n, float* inp, float* targ, float rate) {
    float *costOutput = malloc(n.nbOutput * sizeof(float));

    for (int o = 0; o < n.nbOutput; o++) {
        costOutput[o] = 2 * (n.neuOutput[o] - targ[o]);

        printf("output %d's cost is : %f\n", o, costOutput[o]);
    }
    
    int w = n.nbWeights - n.nbOutput * n.nbHidden[n.nbLayers - 1];
    for (; w < n.nbWeights; w++) {
        
        float Cost = costOutput[w % n.nbOutput];
        printf("w n%d : Cost output nb %d = %f", w, w % n.nbOutput, Cost);

        float primeAct = primeOfActivation(n.neuOutput[w % n.nbOutput]);
        printf(", primeAct = %f", primeAct);
        
        float input    = n.neuHidden[w / n.nbHidden[n.nbLayers - 1]];
        printf(", inp n%d, value=%f", w / n.nbHidden[n.nbLayers - 1], input);

        printf("\n");
    }


    free(costOutput);
}

