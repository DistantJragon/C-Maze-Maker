#include "imager.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

/*
 * This function initializes an image_t struct with the given width, height, and channels.
 */

image_t* image_init(uint width, uint height, uint channels) {
    image_t* image = malloc(sizeof(image_t));
    assert(image);
    image->width = width;
    image->height = height;
    image->channels = channels;
    image->data = calloc(sizeof(unsigned char),
                         (image->width) * \
                         (image->height) * \
                         (image->channels));
    assert(image->data);
    return image;

} /* image_init() */

/*
 * This function frees the memory allocated for the image_t struct.
 */

void image_destroy(image_t* image) {
    free(image->data);
    image->data = NULL;
    free(image);
    image = NULL;
} /* image_destroy() */

/*
 * This function sets the pixel range given by x, y, and w to the given value.
 */

void image_set_range(image_t* image, uint x, uint y, uint w, uint value) {
    memset(image->data + (y * image->width + x) * image->channels, value, w * image->channels);
} /* image_set_range() */

/*
 * This function sets the pixel range given by x, y, w, and h to the given value.
 */

void image_set_rect(image_t* image, uint x, uint y, uint w, uint h, uint value) {
    unsigned char* data = image->data;
    uint channels = image->channels;
    for (int i = y; i < y + h; i++) {
        memset(data + (i * image->width + x) * channels, value, w * channels);
    }
} /* image_set_rect() */

/*
 * This function sets the pixel range given by the image_rect_t's to the given value.
 */

void image_set_rects(image_t* image, image_rect_t* rects, uint num_rects) {
    unsigned char* data = image->data;
    int channels = image->channels;
    for (int i = 0; i < num_rects; i++) {
        for (int j = rects[i].y; j < rects[i].y + rects[i].h; j++) {
            memset(data + (j * image->width + rects[i].x) * channels, rects[i].value, rects[i].w * channels);
        }
    }
} /* image_set_rects() */

/*
 * This function saves the image to the given filename.
 */

void image_save(image_t* image, const char* filename) {
    stbi_write_png(filename, image->width, image->height, image->channels, image->data, image->width * image->channels);
} /* image_save() */
