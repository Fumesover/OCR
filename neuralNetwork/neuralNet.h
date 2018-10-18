#pragma once

typedef struct {
    // Statics
    int nbInputs;   // <= number of input neurons
    int nbLayers;   // <= number of hidden layers
    int *nbHidden;  // <= number of hidden neurons for each hidden layer
    int ttHidden;   // <= total number of hidden neurons
    int nbOutput;   // <= number of output neurons

    // To Compute
    int nbWeights;  // <= number of weights in the network
    int nbBiais;    // <= number of biais in the network

    // Values in arrays
    float *weights; // <= array of all weights
    float *biais;   // <= array of all biais

    // Neurons
    float *neuHidden; // <= array of all hidden neurons
    float *neuOutput; // <= array of all output neurons
}
neuNet;

// Create a neural network
neuNet* NNinit(int nbInputs, int nbLayers, int *nbHidden, int nbOutput);

// Init nerwork with random weights and biais
void NNrand(neuNet* nn);

// Train with one set, return output error
float NNTrain(neuNet* n, float* inp, float* targ, float update);

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

