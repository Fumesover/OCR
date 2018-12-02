#ifndef QUEUE_H
#define QUEUE_H


/*****************************************************/
/*                   STRUCTURES                      */
/*****************************************************/

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


/*****************************************************/
/*                   INITS                           */
/*****************************************************/

// Returns a new empty Elt
Elt *NewElt();

// Returns a new empty Tuple
Tuple *NewTuple();

// Enqueue an Element containing the tuple "data"
// in the Queue "queue"
void Enqueue(Queue *queue, Tuple *data);


#endif
