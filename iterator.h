#ifndef ITERATOR_H
#define ITERATOR_H

#include "image.h"

typedef struct iterator_2D{
    float * array;
    unsigned int x,y;
} iterator_2D;

iterator_2D * iterator_2D_create(float * array);
void iterator_2D_destroy(iterator_2D * iterator);
iterator_2D * iterator_2D_rewind(iterator_2D * iterator);
iterator_2D * iterator_2D_next(iterator_2D * iterator);
pixel_t * iterator_2D_get(iterator_2D * iterator, unsigned int x, unsigned int y);

typedef struct iterator_Z {
    float * array;
    unsigned int x,y;
    int flag_up;
    int flag_exception;
} iterator_Z;


iterator_Z * iterator_Z_create(float * array);
void iterator_Z_destroy(iterator_Z * iterator);
iterator_Z * iterator_Z_rewind(iterator_Z * iterator);
iterator_Z * iterator_Z_next(iterator_Z * iterator);
pixel_t * iterator_Z_get(iterator_Z * iterator, unsigned int x, unsigned int y);


#endif
