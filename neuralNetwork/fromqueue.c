#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../imageTreatment/segmentation.h"
#include "../imageTreatment/image.h"
#include "../imageTreatment/matrix.h"
#include "../imageTreatment/queue.h"
#include "neuralNet.h"
#include "DataSet.h"

char* extractstring(char* filepath, Queue *q) {
    char *s;
    Elt *curr = NULL;
    int **c;
    //Pixel **m;
    //FILE* file = fopen("out.treated","w");
    int t = 0;

    s = malloc(sizeof(char) * 1000);

    if (q->first != NULL)
        curr = q->first;

    while (curr != NULL && curr->data != NULL) {
        c = curr->data->data;

        if (curr->data->width > 1 && curr->data->height > 1){
            s[t] = 'c';
            t++;

            int h = curr->data->height;
            int w = curr->data->width;
        }

        else if (c != NULL && c[0][0] == 38) {
            s[t] = '\n';
            t++;
        }
        else if (c != NULL && c[0][0] == 32) {
            s[t] = ' ';
            t++;
        }
        else
        {
            s[t] = '\n';
            t++;
        }

        curr = curr->next;
    }

    return s;
}
