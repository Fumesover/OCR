
typedef struct {
    // Statiques
    int nbInputs;  // <= nb noeuds entree
    int nbLayers;  // <= nb niveaux caches
    int *nbHidden; // <= nb hidden de chaque lvl
    int nbOutput;  // <= nb noeuds de sortie

    // To Compute
    int nbWeights; 
    int nbBiais;
} 
neuNet;

// for test purposes
int NNmain(void);

// init a NN with 
neuNet NNinit(const int nbInputs, const int nbLayers,
                const int *nbHidden, const int nbOutput);

// random init

// Sigmoid 

/* Sigmoide = 1 / (1 + e ^ -x)
 * 
 * (Regarder SoftMax function (cf livret rose))
 * (Regarder aussi ReLU : max(0,x) 
 *    ^ plus proche du naturel MAIS besoin de pronfondeur)
*/





// load NN 

// save NN 


