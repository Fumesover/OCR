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

// Create a NN
neuNet* NNinit(const int nbInputs, const int nbLayers,
                int *nbHidden, const int nbOutput);

// Init nerwork with random weights and biais
void NNrand(neuNet* nn);

// Train with one set, return output error
float NNTrain(neuNet* n, float* inp, float* targ, float update);

// Compute output of neural network with specific input
const float* NNinput(neuNet* n, float* inp);

// Compute error
float NNerror(neuNet* n, float* target);

// Make a guess with input
float* NNGuess(neuNet* n, float* inp);

// Free NN
void NNfree(neuNet* n);

// load NN
neuNet* NNload(char* filename);

// save NN
void NNsave(neuNet* n, char* filename);

