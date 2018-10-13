#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

void Enqueue(Queue *queue, int **data)
{
    //Create new pointer to new element
    Elt *new = malloc(sizeof(*new));
    new->data = data;
    new->next = NULL;

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
    new->data = 0;
    new->next = NULL;

    return new;
}