#ifndef QUEUE_H
#define QUEUE_H

typedef struct Tuple Tuple;
struct Tuple {
    int **data;
    int height;
    int width;
};

typedef struct Elt Elt;
struct Elt {
    Tuple *data;
    Elt *next;
};

typedef struct Queue Queue;
struct Queue {
    Elt *first;
};


void Enqueue(Queue *queue, Tuple *data);
Elt *NewElt();
Tuple *NewTuple();

#endif
