#ifndef IMAGE_H
#define IMAGE_H

// pixel are stored as char
// pgm value are unsigned char
// the stored value is converted using -128 during reading a pgm and +128 when writing pgm
// compressed file stores the signed value
typedef char pixel_t;
typedef struct {
  int w;
  int h;
  int size; //! compressed size
  pixel_t *data;
} image;


// !! PERFORM img->data malloc
void readPgm(const char *filename, image *img);

// !! DO NOT free img->data (in case of reuse by the caller)
void writePgm(const char *filename, const image *img);


// !! PERFORM img->data malloc to store the readed img->size values
void readCompressed(const char *filename, image *img);

// write ASCII img->w img->h img->size and binary img->data (compresssed size value)
// !! DO NOT free img->data (in case of reuse by the caller)
void writeCompressed(const char *filename, const image *img);

image * img_create(int height, int width);

image * img_empty_cpy(image * img);

void image_init_from(image * img, image * cpy);

#endif

