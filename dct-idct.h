#ifndef DCT_IDCT_H
#define DCT_IDCT_H

// compute dct for a 8 by 8 bloc of img, store the result in data (which has to be already allocated
// the bloc of pixels start at xpos, ypos in img
void dct(image *img, float *data, const int xpos, const int ypos);

// compute idct for a 8 by 8 bloc data, store the result in img (which has to be already allocated)
// the bloc of pixels is stored at xpos, ypos in img
// result value si clamped between -127 to 128
void idct(image *img, float *data, const int xpos, const int ypos);

void idct_opti(image *tga, float data[8*8], const int xpos, const int ypos);
#endif
