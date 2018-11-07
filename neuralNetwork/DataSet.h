typedef struct {
    float* inputs;
    float* output;
    int value;
} Data;

typedef struct {
    Data** rows;
    int    nblines;
    int    nbinputs;
    int    nboutputs;
} DataSet;

void datasetfree(DataSet* ds);

DataSet* getFromFile(char* path);

void shuffle(DataSet* d);
