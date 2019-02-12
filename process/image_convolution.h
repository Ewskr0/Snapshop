#include <gtk-3.0/gtk/gtk.h>
#include <err.h>
#include <stdlib.h>
#include "image_process.h"

#ifndef SNAPSHOP_IMAGE_CONVOLUTION_H
#define SNAPSHOP_IMAGE_CONVOLUTION_H

struct convolution_matrix{
    int     size, divisor;
    int*   matrix;
};

guchar Truncate_256(int value);
guchar Truncate_256_double(double value);
GdkPixbuf* Apply_matrix(GdkPixbuf* image, struct convolution_matrix* matrix);
struct convolution_matrix* InitMotionBlur(int size, int val);
GdkPixbuf* Motion_blur_hor(GdkPixbuf* image, int size);

#endif
