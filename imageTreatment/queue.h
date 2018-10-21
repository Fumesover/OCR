#ifndef QUEUE_H
#define QUEUE_H

// The data of the elements of the queue are Tuple
// containing the character matrix, its height and width
typedef struct Tuple Tuple;
struct Tuple {
    int **data;
    int height;
    int width;
};

// Element of the chained queue
typedef struct Elt Elt;
struct Elt {
    Tuple *data;
    Elt *next;
};

// Chained queue
typedef struct Queue Queue;
struct Queue {
    Elt *first;
};


void Enqueue(Queue *queue, Tuple *data);
Elt *NewElt();
Tuple *NewTuple();

#endif
