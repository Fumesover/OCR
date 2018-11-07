#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "DataSet.h"
#include <err.h>

Data* init_data(size_t nbInp, size_t nbOut) {
    Data* d = malloc(sizeof(Data));
    d->inputs = malloc (nbInp * sizeof(float));
    d->output = malloc (nbOut * sizeof(float));
    return d;
}

void datasetfree(DataSet* ds) {
    for (int i = 0; i < ds->nblines; i++) {
        Data* d = ds->rows[i];
        free(d->inputs);
        free(d->output);
        free(d);
    }

    free(ds->rows);
    free(ds);
}

DataSet* getFromFile(char* path) {
    FILE* fp = fopen(path,"r");

    if (fp == NULL) {
        return malloc(sizeof(DataSet));
    }

    char* line = NULL;
    size_t len = 0;
    size_t read;

    DataSet* d = malloc(sizeof(DataSet));

    if ((read = getdelim(&line, &len, ' ', fp)) != (size_t) -1)
        d->nblines = atoi(line);
    if ((read = getdelim(&line, &len, ' ', fp)) != (size_t) -1)
        d->nbinputs = atoi(line);
    if ((read = getdelim(&line, &len, '\n', fp)) != (size_t) -1)
        d->nboutputs = atoi(line);

    d->rows    = malloc(d->nblines * sizeof(d->rows));
    for (int i = 0; i < d->nblines; i++)
        d->rows[i] = init_data(d->nbinputs, d->nboutputs);

    int pos = -1;
    while (++pos < d->nblines) {
        for (int i = 0; i < d->nbinputs; i++)
            if ((read = getdelim(&line, &len, ' ', fp)) != (size_t) -1)
                d->rows[pos]->inputs[i] = atof(line);

        for (int i = 0; i < d->nboutputs - 1; i++)
            if ((read = getdelim(&line, &len, ' ', fp)) != (size_t) -1)
                d->rows[pos]->output[i] = atof(line);

        if ((read = getdelim(&line, &len, '\n', fp)) != (size_t) -1)
            d->rows[pos]->output[d->nboutputs - 1] = atof(line);
    }

    fclose(fp);
    if (line)
        free(line);
    return d;
}

void shuffle(DataSet* d) {
    int n = d->nblines;
    if (n > 1) {
        for (int i = 0; i < n - 1; i++) {
            int j = i + rand() / (RAND_MAX / (n - i) + 1);
            Data* tmp = d->rows[j];
            d->rows[j] = d->rows[i];
            d->rows[i] = tmp;
        }
    }
    return;
}

DataSet* datasetMerge(DataSet* a, DataSet* b) {
    a = b; b = a;
    errx(1, "TODO");
}
