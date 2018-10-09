#pragma once

typedef struct {
    // Statiques
    int nbInputs;  // <= nb noeuds entree
    int nbLayers;  // <= nb niveaux caches
    int *nbHidden; // <= nb hidden de chaque lvl
    int ttHidden;  // <= nb total hidden neurons
    int nbOutput;  // <= nb noeuds de sortie

    // To Compute
    int nbWeights; 
    int nbBiais;

    // Values in arrays
    float *weights;
    float *biais;
    
    // Neurons
    float *neuHidden;
    float *neuOutput;
} 
neuNet;

// for test purposes
int NNmain(void);

// Create a NN 
neuNet* NNinit(const int nbInputs, const int nbLayers,
                int *nbHidden, const int nbOutput);

// random init
void neuNetRandom(neuNet* nn);

// Forward Propagation
void forwardPropagation(neuNet* n, float* inp);

// One Layer Propagation
void oneLayerPropagation(float* previous, const int pStart, const int pEnd,
                         float* weights,  const int wStart,
                         float* biais,    const int bStart,
                         float* destination, const int dStart, const int dEnd);

/* Sigmoid = 1 / (1 + e ^ -x)
 * 
 * (Regarder SoftMax function (cf livret rose))
 * (Regarder aussi ReLU : max(0,x) 
 *    ^ plus proche du naturel MAIS besoin de pronfondeur)
*/

// Back Propagation
void backPropagation(neuNet* n, float* inp, float* targ, float rate);

// Train with one set
float NNTrain(neuNet* n, float* inp, float* targ, float update);

// Compute error
float NNerror(neuNet* n, float* target);

// Free NN
void freeNeuNet(neuNet* n);

// load NN 

// save NN 


