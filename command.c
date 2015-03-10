#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <xmmintrin.h>

#include "command.h"
#include "dct-idct.h"
#include "image.h"
#include "iterator.h"

//matrice de quantification
static const float qMatrix[8*8] =
{16, 11, 10, 16, 24, 40, 51, 61,

    12, 12, 14, 19, 26, 58, 60, 55,

    14, 13, 16, 24, 40, 57, 69, 56,

    14, 17, 22, 29, 51, 87, 80, 62,

    18, 22, 37, 56, 68, 109, 103, 77,

    24, 35, 55, 64, 81, 104, 113, 92,

    49, 64, 78, 87, 103, 121, 120, 101,

    72, 92, 95, 98, 112, 100, 103, 99};


/********************************* UTILS ***************************/

inline pixel_t * get_pixel(image * img, int x, int y)
{
    return &(img->data[y*img->w +x]);
}


void copy(image * img, float data[8 * 8], const int xpos, const int ypos)
{
    for(int y = 0; y < 8; ++y)
    {
        for(int x = 0; x < 8; ++x)
        {            

            *get_pixel(img, x+xpos, y+ypos) = (pixel_t) (data[x + 8*y]);
        }
    }

}



/****************************** DCT *********************************/

void apply_dct(image * img, image * out, quantization_function quantifier) 
{

    float * data = calloc(64,sizeof(float));

    for(int y = 0; y < img->h; y+=8)
    {
        for(int x = 0; x < img->w; x+=8)
        {
            dct(img, data, x, y);
            quantifier(data);
            copy(out, data, x, y);
        }
    }
    free(data);
}

void normalization(float data[8 * 8])
{
    for(int i = 0; i < 64 ; ++i)
    {
        data[i] = (data[i] / 8.f);
    }

}

void quantify(float data[8*8])
{

    for(int i = 0 ; i < 64 ; ++i)
    {
        data[i]= round((data[i]/qMatrix[i]));
    }

}


/****************************** VECTORIZE ***************************/

void apply_dct_vectorize(image * img, image * out)
{

    float * data = calloc(64,sizeof(float));
    float * vector_z = calloc(64, sizeof(float));
    int index = 0;
    for(int y = 0; y < img->h; y+=8)
    {
        for(int x = 0; x < img->w; x+=8)
        {
            dct(img, data, x, y);
            quantify(data);
            vectorize(data,vector_z);
            copy_vector(out, vector_z, index);
            index+=64;
        }
    }
    free(data);
    free(vector_z);

}

void vectorize(float data[8*8], float vector_z[8*8])
{

    iterator_Z * iterator = iterator_Z_create(data);

    //change this into whille(iterator_Z_valid(iterator))
    for(int i = 0; i < 64; ++i)
    {
        vector_z[i] = data[iterator->x + (iterator->y)*8];
        iterator = iterator_Z_next(iterator);
    }

    iterator_Z_destroy(iterator);
}

void devectorize(float data[8*8], float vector_z[8*8])
{
    iterator_Z * iterator = iterator_Z_create(data);

    //change this into while(iterator_Z_valid(iterator))
    for(int i = 0; i < 64; ++i)
    {
        data[iterator->x + (iterator->y)*8] = vector_z[i] ;
        iterator = iterator_Z_next(iterator);
    }

    iterator_Z_destroy(iterator);
}

int img_to_vector(image * img, float data[8*8], int pos)
{

    // represente la valeur du prochain vecteur, ce sera la valeur retourne qui 
    // sera utilise dans le prochain appel
    int index_max = img->data[pos]+1+pos;
    for(int i = pos; i < index_max; ++i)
    {
        data[i-pos] = img->data[i];
    }
    // le calloc ayant initialise le tableau a zero, pas besoin de parcourir le tableau
    // plus loin.
    return index_max;

}


/****************************** COMPRESS ***************************/
void compress(image * img, image * out)
{
    float * data = calloc(64,sizeof(float));
    float * vector_z = calloc(64, sizeof(float));
    int index = 0;
    int nb_value = 0;

    for(int y = 0; y < img->h; y+=8)
    {
        for(int x = 0; x < img->w; x+=8)
        {
            
            dct(img, data, x, y);
            quantify(data);
            vectorize(data,vector_z);

            nb_value = non_zero_value_pos(vector_z);
            copy_compress(out, vector_z, nb_value, index);

            index+=nb_value+1;
        }
    }
    out->size = index;
    free(data);
    free(vector_z);


}
/****************************** DECOMPRESS *************************/
void decompress(image * img, image * out)
{
    float * data = calloc(64,sizeof(float));
    float * vector_z = calloc(64, sizeof(float));
    int pos = 0;

    for(int y = 0; y < img->h; y+=8)
    {
        for(int x = 0; x < img->w; x+=8)
        {

            pos = img_to_vector(img, vector_z, pos);

            fill_zero(vector_z);

            devectorize(data,vector_z);

            unquantify(data);

            idct(out, data, x, y);

        }
    }
    
    free(data);
    free(vector_z);
    
}

void unquantify(float data[8*8])
{
    __m128 * mData = (__m128 *) data;
    __m128 * matrix = (__m128 *) qMatrix;
    
    // __m128 is used as an array of 4 float
    // so we only do 64/4 loops = 16 loops
    for(int i = 0; i < 16; ++i)
    {
        (*mData) = _mm_mul_ps(*mData, *matrix);
        mData++;
        matrix++;
    }
}

void fill_zero(float vector[8*8])
{
    // We have to remove the first value indicating the number of significant values
    // then we have to fill the vector with zero
    int nb = vector[0];
    for(int i = 1; i <= nb; ++i)
    {
        vector[i-1] = vector[i];
    }

    // the memset is not usefull since we have used a calloc instead of a malloc
    memset(&vector[nb],0, (64-nb) * sizeof(float));

}

int copy_compress(image * img,float vector[8*8],int nb_value, int pos)
{
    img->data[pos++] = nb_value;
    for(int i = 0; i < nb_value ; ++i)
    {
        img->data[pos++] = vector[i];
    }
    return pos;
}

void copy_vector(image * img, float vector[8*8],const int index)
{
    for(int i = 0; i < 64; ++i)
    {
        img->data[index+i] = (pixel_t)vector[i];
    }
}

int non_zero_value_pos(float vector[8*8])
{
    int index = 63;

    while( ((int) vector[index] )==0 && index >= 0)
    {
        --index;
    }

    return index+1;
}

/********************************* ERROR ***************************/

long calcul_diff(image * img)
{
    image * cpy = img_empty_cpy(img);
    image * res = img_empty_cpy(img);
    compress(img, cpy);
    decompress(cpy, res);

    long size = img->w * img->h;
    long diff = 0;
    int tmp = 0;
    for(long i = 0; i < size; ++i)
    {
        tmp = img->data[i] - res->data[i];
        diff += tmp*tmp;
    }

    free(cpy->data);
    free(cpy);
    free(res->data);
    free(res);
    diff = diff/(img->w * img->h);
    return diff;

}

