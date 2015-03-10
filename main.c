#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

#include <time.h>

#include "image.h"
#include "dct-idct.h"
#include "command.h"
#include "iterator.h"

typedef struct{
    int compress;
    char inFilename[256];
    char outFilename[256];
} s_args;

void parseArgs(char *argv[], s_args* args){
    args->compress = atoi(argv[1]);
    strncpy(args->inFilename, argv[2], 256);
    strncpy(args->outFilename, argv[3], 256);
}

void usage(char * progname) {
    printf("usage : %s mode in out\n", progname);
    printf("mode \t 0 : decompression, 1 : compression, 2 : save dct (pgm format),\n\t 3 : save quantize (pgm format), 4 : save vectorize (xxx format), 5 output compression loss\n");
    printf("in : input filename, pgm if compression, save dct or save quantize, xxx if decompression\n");
    printf("out : output filename, xxx if compression, pgm if decompression, save dct or save quantize\n");
    exit(EXIT_FAILURE);

}

void test_compress(image * img)
{
  for(int y = 0; y < img->h ; y++)
  {
    for(int x = 0; x < img->w ; x++)
    {
      printf("%d ",img->data[x+(img->w * y)]);
    }
    printf("\n");
  }
}
int main(int argc, char *argv[]){
    s_args args;
    if(argc!=4){
        usage(argv[0]);
    }
    else{
        parseArgs(argv, &args);
    }

    image img;
    image output;

    switch(args.compress) {
        case 0 :
            // decompression

            readCompressed(args.inFilename,&img);
            image_init_from(&img, &output);
            decompress(&img, &output);
            writePgm(args.outFilename, &output);


            break;
        case 1 :
            // compression

            readPgm(args.inFilename, &img);
            image_init_from(&img, &output);
            compress(&img, &output);
            writeCompressed(args.outFilename, &output);

            break;
        case 2 :
            // test dct
            readPgm(args.inFilename,&img);
            image_init_from(&img,&output);
            apply_dct(&img, &output,normalization);
            writePgm(args.outFilename,&output);

            break;
        case 3 :
            // test quantify
            readPgm(args.inFilename,&img);
            image_init_from(&img, &output);
            apply_dct(&img, &output,quantify);
            writePgm(args.outFilename,&output);

            break;
        case 4 :
            // test vectorize

            readPgm(args.inFilename,&img);
            image_init_from(&img, &output);
            apply_dct_vectorize(&img, &output);
            writePgm(args.outFilename,&output);
            break;
        case 5 :
            // compute and print error
            /*
            readPgm(args.inFilename, &img);

            long diff = calcul_diff(&img);
            printf("%ld \n",diff);
            */
            readCompressed(args.inFilename, &img);
        test_compress(&img);
            break;
       
        default :
            usage(argv[0]);
    }
    return 0;
}
