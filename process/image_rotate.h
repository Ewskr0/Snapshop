#include <gtk-3.0/gtk/gtk.h>
#include <err.h>
#include "image_process.h"

#ifndef SNAPSHOP_IMAGE_ROTATE_H
#define SNAPSHOP_IMAGE_ROTATE_H
GdkPixbuf* Rotate(GdkPixbuf *image, int n);
#endif
