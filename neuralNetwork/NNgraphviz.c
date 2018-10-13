#include <stdio.h>
#include "neuralNet.h"
#include <stdlib.h>

void NNtoGraphviz(neuNet* n, float* input);
char* getColor(float v);

int main(void) {
    
    int hidden[] = {3,4};

    neuNet* n = NNinit(3, 2, hidden, 3);

    NNrand(n);
    
    float inputs[] = {1.0f, 0.0f, 0.0f}; 
    float target[] = {1.0f, 1.0f, 0.0f};

    for (int i = 0; i < 1000; i++)
        NNTrain(n, inputs, target, 0.1f);

    NNtoGraphviz(n, inputs); 

    NNfree(n);
    
    /*const char* color = getColor(1.0f);
    printf("%s", color);
    getColor(0.75f);
    getColor(0.50f);
    getColor(0.25f);
    getColor(0.0f);
    */
    return 0;
}

char* getColor(float v) {
    unsigned int rgb[] = {0, 0, 0};

    if (v <= 0.5) {
        v *= 2.0;
        rgb[0] = (unsigned int)(255 * (1.0 - v) + 0.5);
        rgb[2] = (unsigned int)(255 * v + 0.5);
    } else {
        v = v * 2.0 - 1.0;
        rgb[0] = (unsigned int)(255 * (1.0 - v) + 0.5);
        rgb[2] = (unsigned int)(255 * (v) + 0.5);
    }
  
    char* out = malloc(7 * sizeof(char));
    sprintf(out, "%06x", rgb[0] + rgb[1] * 256 + rgb[2] * 256 * 256);
    return out;
}

void printsubgraph(float* n, float* vals, int nCount, int nStart, int vStart, 
                    int nameStart, char* label, int layer) {

    printf("\tsubgraph cluster_%d {\n", layer);
    printf("\t\tcolor=white;\n");
    printf("\t\tlabel=\"%s\";\n", label);
    printf("\t\tnode [color=chartreuse, style=filled, shape=circle];\n");
    
    if (n[0] > -1.0f) { 
        for (int i = 0; i < nCount; i++) {
            char* color = getColor(vals[vStart + i]);

            printf("\t\tx%d [color=\"#%s\", fillcolor=\"#%s\"];\n", 
                     nameStart + i, color, color);
        
            free(color);
        }
    } else { 
        for (int i = 0; i < nCount; i++) {
            char* color = getColor(vals[nStart + i]);

            printf("\t\tx%d [color=\"#%s\", fillcolor=\"#%s\", label=\"\"];\n",
                nameStart + i, color, color);

            free(color);
        }
    }

    printf("\t}\n");
}

void printLinks(int inpCount, int inpOffset, int outCount, int outOffset) {
    for (int i = 0; i < inpCount; i++) {
        for (int j = 0; j < outCount; j++) {
            printf("\tx%d -> x%d;\n", i + inpOffset, j + outOffset);
        }
    }
}

void NNtoGraphviz(neuNet* n, float* input){
    printf("digraph G {\n");
    printf("\trankdir=LR;\n");
    printf("\tedge[style=solid, tailport=e, headport=w];\n");
    printf("\n");
    printf("\tsplines=line;\n");
    printf("\tnodesep=\".05\";\n");
    printf("\tnode [label=\"\"];\n"); 
    printf("\n");
    
    float nope[] = {-1.0f};
    printsubgraph(nope, input, n->nbInputs, 0, 0, 0, "Input Layer", 0);

    printf("\n");
    
    int nameStart = n->nbInputs;
    int nStart = 0;
    for (int i = 0; i < n->nbLayers; i++) {
        
        printsubgraph(n->biais, n->neuHidden , n->nbHidden[i], nStart, nStart, nameStart, "Hidden Layer", i + 1);
        
        nStart    += n->nbHidden[i];
        nameStart += n->nbHidden[i];

        printf("\n");
    }
    

    printsubgraph(n->biais, n->neuOutput, n->nbOutput, nStart, 0, nameStart, "Output Layer", n->nbLayers + 2);
    
    printf("\n");

    printLinks(n->nbInputs, 0, n->nbHidden[0], n->nbInputs);

    int offset = n->nbInputs;
    for (int i = 0; i < n->nbLayers - 1; i++) {
        printLinks(n->nbHidden[i], offset, n->nbHidden[i + 1], offset + n->nbHidden[i]);
        offset += n->nbHidden[i];
    }
    
    printLinks(n->nbHidden[n->nbLayers - 1], offset, n->nbOutput, offset + n->nbHidden[n->nbLayers - 1]);

    printf("}\n");
}

