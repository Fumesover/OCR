#include "neuralNet.h"

#include <stdio.h>

int NNmain(void){
    
    //printf("test");

    int hidden[] = {2,2};

    neuNet n = NNinit(2, 2, hidden, 2);

    printf("nbInputs : %d\n", n.nbInputs);
    printf("nbOutput : %d\n", n.nbOutput);
    
    printf("nbLayers : %d\n", n.nbLayers);

    for(int i = 0; i < 2; i++){
        printf("layer %d : nbHidden : %d\n", i, n.nbHidden[i]);
    }
    
    printf("nbWeights : %d\n", n.nbWeights);
    printf("nbBiais : %d\n", n.nbBiais);

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

    // Calcul nb poids
    int p = 0;
    nn.nbWeights = nbInputs * nbHidden[p];
    
    for(; p < nbLayers - 1; p++) {
        nn.nbWeights += nbHidden[p] * nbHidden[p + 1];
    }
    
    nn.nbWeights += nbHidden[p] * nbOutput;

    // Calcul nb biais
    nn.nbBiais = 0;
    for(int p = 0; p < nbLayers; p++) {
        nn.nbBiais += nbHidden[p];
    }
    nn.nbBiais += nbOutput;
    

    return nn; 
}
