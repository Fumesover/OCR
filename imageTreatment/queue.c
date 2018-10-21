#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

/*** INIT ***/

//Returns a new empty Element
Elt *NewElt()
{
    Elt *new = malloc(sizeof(*new));
    new->data = NULL;
    new->next = NULL;

    return new;
}

// Returns a new empty Tuple
Tuple *NewTuple()
{
    Tuple *new = malloc(sizeof(*new));
    new->data = NULL;
    new->height = 0;
    new->width = 0;

    return new;
}

// Enqueue an Element containing the tuple "data"
// in the Queue "queue"
void Enqueue(Queue *queue, Tuple *data)
{
    // New element
    Elt *new = NewElt();
    new->data = data;

    // If queue not empty, we travel to the end of the queue
    if(queue->first != NULL)
    {
        Elt *this = queue->first;
        while (this->next != NULL)
            this = this->next;
        this->next = new;
    }

    // New element is the first if queue is empty
    else
        queue->first = new;
}
