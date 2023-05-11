#ifndef __imager_h__
#define __imager_h__

#include "common_typedefs.h"

typedef struct image_s {
    uint width;
    uint height;
    uint channels;
    byte* data;
} image_t;

typedef struct image_rect_s {
    uint x;
    uint y;
    uint w;
    uint h;
    uint value;
} image_rect_t;

image_t* image_init(uint width, uint height, uint channels);
void     image_destroy(image_t* image);
void     image_save(image_t* image, const char* filename);
void     image_set_range(image_t* image, uint x, uint y, uint w, uint value);
void     image_set_rect(image_t* image, uint x, uint y, uint w, uint h, uint value);
void     image_set_rects(image_t* image, image_rect_t* rects, uint num_rects);

#endif
