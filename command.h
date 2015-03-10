#ifndef COMMAND_H
#define COMMAND_H
#include "image.h"

pixel_t * KpixelPtr(image * img, int x, int y);

typedef void (* quantization_function)(float[8*8]);

// divide each value of data by 8.f
void normalization(float data[8 * 8]);

// quantify using a matrix
void quantify(float data[8*8]);
void unquantify(float data[8*8]);

// copy each value of data into img->data at the right position
void copy(image * img, float data[8 * 8], const int xpos, const int ypos);

void apply_dct(image * img, image * out, quantization_function quantifier);

void apply_dct_vectorize(image * img, image * out);

void vectorize(float data[8*8],float vector_z[8*8]);
void devectorize(float data[8*8],float vector_z[8*8]);

int non_zero_value_pos(float vector[8*8]);

void compress(image * img, image * out);

int copy_compress(image * img,float vector[8*8],int nb_value, int pos);

void copy_vector(image * img, float vector[8*8],const int index);

void decompress(image * img, image * out);

void decompress_opti(image * img, image * out);

long calcul_diff(image * img);

void fill_zero(float vector[8*8]);
#endif

