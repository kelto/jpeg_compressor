#include "image.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

// from http://ugurkoltuk.wordpress.com/2010/03/04/an-extreme-simple-pgm-io-api/
void skipComments(FILE *fp)
{
    int ch;
    char line[100];
 
    while ((ch = fgetc(fp)) != EOF && isspace(ch))  ;

    if (ch == '#') {
        char *p = fgets(line, sizeof(line), fp);
	if(!p){
	  fprintf(stderr, "Could not read file\n");
	  exit(EXIT_FAILURE);
	}

        skipComments(fp);
    } else
        fseek(fp, -1, SEEK_CUR);

}

void readPgm(const char *filename, image *img){
  FILE *pgmFile;
  char version[3];
  int i, j;
  int lo;
  
  pgmFile = fopen(filename, "rb");
  if (pgmFile == NULL) {
    perror("cannot open file to read");
    fprintf(stderr, "%s\n", filename);
    exit(EXIT_FAILURE);
  }
  
  char *p = fgets(version, sizeof(version), pgmFile);
  if(!p){
    fprintf(stderr, "Could not read file\n");
    exit(EXIT_FAILURE);
  }
  if (strcmp(version, "P5")) {
    fprintf(stderr, "Wrong file type!\n");
    exit(EXIT_FAILURE);
  }
  
  skipComments(pgmFile);
  int scanCount = 0;
  scanCount = fscanf(pgmFile, "%d", &img->w);
  if(scanCount != 1){
    fprintf(stderr, "Could not read file\n");
    exit(EXIT_FAILURE);
  }
  skipComments(pgmFile);
  scanCount=fscanf(pgmFile, "%d", &img->h);
  if(scanCount != 1){
    fprintf(stderr, "Could not read file\n");
    exit(EXIT_FAILURE);
  }

  skipComments(pgmFile);
    int max_value;
    scanCount =  fscanf(pgmFile, "%d", &max_value);
    if(scanCount != 1){
      fprintf(stderr, "Could not read file\n");
      //exit(EXIT_FAILURE);
    }

    // skip CR
    fgetc(pgmFile);
    assert(max_value == 255);

    img->data = malloc(sizeof(pixel_t)*img->w*img->h);
    for (i = 0; i < img->h; ++i)
      for (j = 0; j < img->w; ++j) {
	lo = fgetc(pgmFile);
	img->data[i*img->w+j] = (char)(lo-128);
      }
 
    fclose(pgmFile);
    return;
}

void writePgm(const char *filename, const image *img)
{
    FILE *pgmFile;
    int i, j;
    int lo;
 
    pgmFile = fopen(filename, "wb");
    if (pgmFile == NULL) {
        perror("cannot open file to write");
	fprintf(stderr, "%s\n", filename);
        exit(EXIT_FAILURE);
    }
 
    fprintf(pgmFile, "P5 ");
    fprintf(pgmFile, "%d %d ", img->w, img->h);
    fprintf(pgmFile, "%d ", 255);

    for (i = 0; i < img->h; ++i)
      for (j = 0; j < img->w; ++j) {
	lo = img->data[i*img->w+j];
	lo += 128;
	fputc(lo, pgmFile);
      }
    fclose(pgmFile);
}
// end of code from http://ugurkoltuk.wordpress.com/2010/03/04/an-extreme-simple-pgm-io-api/

void readCompressed(const char *filename, image *img){
  FILE *pgmFile;
  int i;
  int lo;
  
  pgmFile = fopen(filename, "rb");
  if (pgmFile == NULL) {
    perror("cannot open file to read");
    exit(EXIT_FAILURE);
  }

  int scanCount = 0;
  scanCount = fscanf(pgmFile, "%d", &img->w);
  if(scanCount != 1){
    fprintf(stderr, "Could not read file\n");
    exit(EXIT_FAILURE);
  }

  scanCount = fscanf(pgmFile, "%d", &img->h);
  if(scanCount != 1){
    fprintf(stderr, "Could not read file\n");
    exit(EXIT_FAILURE);
  }

  scanCount = fscanf(pgmFile, "%d", &img->size);
  if(scanCount != 1){
    fprintf(stderr, "Could not read file\n");
    exit(EXIT_FAILURE);
  }

  //skip CR
  fgetc(pgmFile);
  img->data = malloc(sizeof(pixel_t)*img->size);
  for (i = 0; i < img->size; ++i){
    lo = (char)fgetc(pgmFile);
    img->data[i] = lo;
  }
  fclose(pgmFile);
  return;
}

void writeCompressed(const char *filename, const image *img){
  FILE *pgmFile;
  int i;
  int lo;
 
  pgmFile = fopen(filename, "wb");
  if (pgmFile == NULL) {
    perror("cannot open file to write");
    exit(EXIT_FAILURE);
  }
 
  fprintf(pgmFile, "%d %d %d\n", img->w, img->h, img->size);
  
  for (i = 0; i < img->size; ++i){
    lo = img->data[i];
    fputc(lo, pgmFile); 
  }
}

image * img_create(int height, int width)
{
    image * img = malloc(sizeof(image));
    img->h = height;
    img->w = width;
    img->data = malloc(sizeof(pixel_t) * height * width);

    return img;
}

image * img_empty_cpy(image * img)
{
    image * cpy = img_create(img->h, img->w);
    cpy->size = img->size;
    return cpy; 
}

void image_init_from(image * img, image * copy)
{
        copy->size = img->size;
        copy->w = img->w;
        copy->h = img->h;
        copy->data = malloc(sizeof(pixel_t)*copy->h*copy->w);

}
