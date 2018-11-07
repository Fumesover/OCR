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

// Get DataSet from file
DataSet* getFromFile(char* path);

// Shuffle the dataset
void shuffle(DataSet* d);

// Free a dataset
void datasetfree(DataSet* ds);

// Merge two DataSets together
DataSet* datasetMerge(DataSet*, DataSet*);
