#include "neuralNet.h"

#include <stdio.h>

int NNmain(void){
    
    printf("test");

    return 0;
}

/* Nb hidden = 
 *              < 2 * nbInputs
 *              nbInput < xxxx < nbOuput (si beaucoup de nodes)
 *              2 / 3 * nbInput + nbOutput (maybe)
 *              (faire un mix de tout ca !)
*/

// On prendra QUE 1 hidden layer pour le coup parce que voila
neuNet NNinit(const int nbInputs, const int nbLayers, const *int nbHidden, const int nbOutput){
    neuNet nn;
    
    nn.nbInputs = nbInputs;
    nn.nbHidden = nbHidden;
    nn.nbOutput = nbOutput;

    nn.nbWeights = nbHidden * (nbInputs + nbOutput);
    nn.nbBiais   = nbHidden + nbOutput;

   return nn; 
}
