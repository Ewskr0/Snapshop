#include <gtk-3.0/gtk/gtk.h>
#include <err.h>
#include "image_process.h"

#ifndef SNAPSHOP_IMAGE_SCALE_H
#define SNAPSHOP_IMAGE_SCALE_H

GdkPixbuf* Scale_neighbors(GdkPixbuf *image, int x_scale, int y_scale);
GdkPixbuf* Scale_bilinear(GdkPixbuf *image, int x_scale, int y_scale);
GdkPixbuf* Truncate(GdkPixbuf* image, struct box* box);

#endif