#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

void Enqueue(Queue *queue, Tuple *data)
{
    //Create new pointer to new element
    Elt *new = NewElt();
    new->data = data;
    //Add new element the end of the queue
    if(queue->first != NULL)
    {
        Elt *this = queue->first;
        while (this->next != NULL)
            this = this->next;
        this->next = new;
    }

    //New element as first element if queue is empty
    else
        queue->first = new;
}

Elt *NewElt()
{
    Elt *new = malloc(sizeof(*new));
    new->data = NULL;
    new->next = NULL;

    return new;
}

Tuple *NewTuple()
{
    Tuple *new = malloc(sizeof(*new));
    new->data = NULL;
    new->height = 0;
    new->width = 0;

    return new;
}

