#include "neuralNet.h"
#include "DataSet.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int getMaxPos(float* arr, int len) {
    int max = 0;
    int pos = -1;
    
    while (++pos < len) 
        if (arr[pos] > arr[max])
            max = pos;
    
    return max;
}

int main (int argc, char* argv[]) {
    char *loadpath          = NULL;
    char *savepath          = NULL;
    int   nbHidden          = 0;
    int nbItt               = 0;
    float updateRate        = 1.0f;
    float anneal            = 1.0f;
    unsigned int finalprint = 0;

    int c;
    while ((c = getopt (argc, argv, "i:d:o:hn:u:a:r:")) != -1) {
        switch (c) {
            case 'd':
                nbHidden = atoi(optarg);
                break;
            case 'i':
                loadpath = optarg;
                break;
            case 'o':
                savepath = optarg;
                break;
            case 'n':
                nbItt = atoi(optarg);
                break;
            case 'u':
                updateRate = atof(optarg);
                break;
            case 'a':
                anneal = atof(optarg);
                break;
            case 'r':
                finalprint = atoi(optarg);
                break;
            case 'h':
            default:
                fprintf(stderr, "Usage: %s [OPTION]... [FILE]...\n", argv[0]);
                fprintf(stderr, "          [-i neuralnetwork]\n");
                fprintf(stderr, "          [-d nbhidden] \n");
                fprintf(stderr, "          [-o neuralnetwork]\n");
                fprintf(stderr, "          [-n number of itterations]\n");
                fprintf(stderr, "          [-u update rate] (default 1)\n");
                fprintf(stderr, "          [-a anneal] (default 1)\n");
                fprintf(stderr, "          [-r pick n results and show them] (default 0)\n");
                exit(1);
        }
    }

    if (optind >= argc)
        errx(1, "Expect argument after options");
    
    DataSet* ds = getFromFile(argv[optind++]);
    while (optind < argc) 
        errx(1, "Error: dataset merge not implemented yet");
    
    if (!ds)
        exit(1);

    neuNet* n = NULL;
    if (loadpath) {
        n = NNload(loadpath);
        if (!n) 
            errx(1, "Invalid path %s", loadpath); 
    } else {
        if (nbHidden == 0) 
            errx(1, "Need at least 1 hidden neuron");
    
        n = NNinit(ds->nbinputs, nbHidden, ds->nboutputs);
        NNrand(n);
    }
    
    while (nbItt-- > 0) {
        shuffle(ds);
        float err = 0.0f;
        
        for (int i = 0; i < ds->nblines; i++) {
            Data* d = ds->rows[i];
            err += NNTrain(n, d->inputs, d->output, updateRate);
        }

        printf("n°%d : batch error : ", nbItt);
        printf("%f -- update rate : %f\n", err, updateRate);
        
        updateRate *= anneal;
    } // while (--nbItt > 0);
    
    if (finalprint) {
        shuffle(ds);

        printf("--\n");
        
        for (unsigned int i = 0; i < finalprint; i++) {
            Data* d = ds->rows[i];
            float* out = NNGuess(n, d->inputs);

            printf("Preficted : %d -- Expects %d\n", 
                    getMaxPos(out, n->nbOutput), 
                    getMaxPos(d->output, n->nbOutput));
        }
    }

    int nbErrors = 0;
    for (int i = 0; i < ds->nblines; i++) {
        Data* d = ds->rows[i];
        float* out = NNGuess(n, d->inputs);

        if (getMaxPos(out, n->nbOutput) != getMaxPos(d->output, n->nbOutput))
            nbErrors++;
    }
    printf("NeuralNet result : %d inputs -> %d errors.\n", ds->nblines, nbErrors);

    if (savepath) {
        NNsave(n, savepath);
    }
    
    datasetfree(ds);
    NNfree(n);
    return 0;
}
