#include <stdlib.h>
#include <stdio.h>
#include "iterator.h"

iterator_Z * iterator_Z_create(float * array)
{
    iterator_Z * iterator = malloc(sizeof(iterator_Z));

    iterator->array = array;

    iterator_Z_rewind(iterator);

    return iterator;
}
void iterator_Z_destroy(iterator_Z * iterator)
{
    free(iterator);
}
iterator_Z * iterator_Z_rewind(iterator_Z * iterator)
{
    iterator->x = 0; 
    iterator->y = 0; 
    iterator->flag_up = 1;
    iterator->flag_exception = 1;
    return iterator;
}
iterator_Z * iterator_Z_next(iterator_Z * iterator)
{
    if(iterator->flag_up)
    {
        if(iterator->flag_exception)
        {
            if(iterator->x == 7)
            {
                iterator->y += 1;
            }
            else
            {
                iterator->x += 1; 
            }
            iterator->flag_exception = 0;
            iterator->flag_up = 0;
        }
        else
        {
            iterator->x += 1;
            iterator->y -= 1;
            if(iterator->y == 0 || iterator->x == 7)
            {
                iterator->flag_exception = 1;
            }
        }
    }
    else
    {
        if(iterator->flag_exception)
        {
            if(iterator->y == 7)
            {
                iterator->x += 1;
            }
            else
            {
                iterator->y += 1; 
            }
            iterator->flag_up = 1;
            iterator->flag_exception = 0;
        }
        else
        {
            iterator->y += 1;
            iterator->x -= 1;

            if(iterator->x == 0 || iterator->y == 7)
            {
                iterator->flag_exception = 1;
            }
        }
    }

    return iterator;
}

