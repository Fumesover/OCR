#include <stdio.h>
#include "neuralNet.h"
#include <stdlib.h>
#include <unistd.h>
#include "NNgraphviz.h"

// void NNtoGraphviz(FILE* output, neuNet* n, float* input);
char* getColor(float v);

/*

int main(int argc, char** argv) {
    int hidden[] = {3,4};

    neuNet* n = NNinit(3, 2, hidden, 3);

    NNrand(n);
    
    float inputs[] = {1.0f, 0.0f, 0.0f}; 
    float target[] = {1.0f, 0.0f, 0.0f};
    
    FILE* output = NULL;

    int opt;
    while ((opt = getopt(argc, argv, "o:")) != -1) {
        switch (opt) {
            case 'o':
                output = fopen(optarg, "w");
                break;
            default:
                break;
        }
    }

    if (!output)
        output = stdout;


    //for (int i = 0; i < 1000; i++)
    NNTrain(n, inputs, target, 0.1f);
    
    n->neuOutput[0] = 0.0f;
    n->neuOutput[1] = 0.5f;
    n->neuOutput[2] = 0.0f;

    // for (int i = 0; i < n->nbOutput; i++)
    //    printf("output[%d] = %f\n", i, n->neuOutput[i]);

    NNtoGraphviz(output, n, inputs); 

    NNfree(n);
    fclose(output); 
    return 0;
}

*/

char* getColor(float v) {
    unsigned int rgb[] = {0, 0, 0};

    /*
    if (v <= 0.5) {
        v *= 2.0;
        rgb[0] = (unsigned int)(255 * (1.0 - v) + 0.5);
        rgb[2] = (unsigned int)(255 * v + 0.5);
    } else {
        v = v * 2.0 - 1.0;
        rgb[0] = (unsigned int)(255 * (1.0 - v) + 0.5);
        rgb[2] = (unsigned int)(255 * (v) + 0.5);
    }
    */
    rgb[0] = 255 * v;
    rgb[1] = 255 * v;
    rgb[2] = 255 * v;
  
    char* out = malloc(7 * sizeof(char));
    sprintf(out, "%06x", rgb[0] + rgb[1] * 256 + rgb[2] * 256 * 256);
    return out;
}

void printsubgraph(FILE* output, float* n, float* vals, int nCount, int nStart,
                    int vStart, int nameStart, char* label, int layer) {

    fprintf(output, "\tsubgraph cluster_%d {\n", layer);
    fprintf(output, "\t\tcolor=white;\n");
    fprintf(output, "\t\tlabel=\"%s\";\n", label);
    fprintf(output, "\t\tnode [color=chartreuse, style=filled, shape=circle];\n");
    
    if (n[0] > -1.0f) { 
        for (int i = 0; i < nCount; i++) {
            char* color = getColor(vals[vStart + i]);

            fprintf(output, "\t\tx%d [color=\"#000000\", fillcolor=\"#%s\"];\n", 
                     nameStart + i, color);
        
            free(color);
        }
    } else { 
        for (int i = 0; i < nCount; i++) {
            char* color = getColor(vals[nStart + i]);

            fprintf(output, "\t\tx%d [color=\"#000000\", fillcolor=\"#%s\", label=\"\"];\n",
                nameStart + i, color);

            free(color);
        }
    }
    
    fprintf(output, "\t}\n");
}

void printLinks(FILE* output, int inpCount, int inpOffset, int outCount, int outOffset) {
    for (int i = 0; i < inpCount; i++) 
        for (int j = 0; j < outCount; j++) 
            fprintf(output, "\tx%d -> x%d;\n", i + inpOffset, j + outOffset);
}

void NNtoGraphviz(FILE* output, neuNet* n, float* input){
    fprintf(output, "digraph G {\n");
    fprintf(output, "\trankdir=LR;\n");
    fprintf(output, "\tedge[style=solid, tailport=e, headport=w];\n");
    fprintf(output, "\n");
    fprintf(output, "\tsplines=line;\n");
    fprintf(output, "\tnodesep=\".06\";\n");
    fprintf(output, "\tnode [label=\"\"];\n"); 
    // fprintf(output, "\tbgcolor=\"transparent\";");
    fprintf(output, "\n");
    
    float nope[] = {-1.0f};
    printsubgraph(output, nope, input, n->nbInputs, 0, 0, 0, "Input Layer", 0);

    fprintf(output, "\n");
    
    int nameStart = n->nbInputs;
    int nStart = 0;
    for (int i = 0; i < n->nbLayers; i++) {
        
        printsubgraph(output, n->biais, n->neuHidden , n->nbHidden[i], 
                    nStart, nStart, nameStart, "Hidden Layer", i + 1);
        
        nStart    += n->nbHidden[i];
        nameStart += n->nbHidden[i];

        fprintf(output, "\n");
    }
    
    printsubgraph(output, n->biais, n->neuOutput, n->nbOutput, 
                   0, 0, nameStart, "Output Layer", n->nbLayers + 2);
    
    fprintf(output, "\n");

    printLinks(output, n->nbInputs, 0, n->nbHidden[0], n->nbInputs);

    int offset = n->nbInputs;
    for (int i = 0; i < n->nbLayers - 1; i++) {
        printLinks(output, n->nbHidden[i], offset, n->nbHidden[i + 1], offset + n->nbHidden[i]);
        offset += n->nbHidden[i];
    }
   
    printLinks(output, n->nbHidden[n->nbLayers - 1], offset, n->nbOutput, offset + n->nbHidden[n->nbLayers - 1]);

    fprintf(output, "}\n");
}

