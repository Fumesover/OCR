#ifndef QUEUE_H
#define QUEUE_H

typedef struct Elt Elt;
struct Elt {
    int **data;
    Elt *next;
};

typedef struct Queue Queue;
struct Queue {
    Elt *first;
};

void Enqueue(Queue *queue, int **data);
Elt *NewElt();

#endif
