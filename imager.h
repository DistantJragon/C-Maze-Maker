#ifndef __imager_h__
#define __imager_h__

typedef struct image_s {
    int width;
    int height;
    int channels;
    unsigned char* data;
} image_t;

typedef struct image_rect_s {
    int x;
    int y;
    int w;
    int h;
    unsigned int value;
} image_rect_t;

image_t* image_init(int width, int height, int channels);
void     image_destroy(image_t* image);
void     image_save(image_t* image, const char* filename);
void     image_set_range(image_t* image, int x, int y, int w, unsigned char value);
void     image_set_rect(image_t* image, int x, int y, int w, int h, unsigned char value);
void     image_set_rects(image_t* image, image_rect_t* rects, int num_rects);

#endif
