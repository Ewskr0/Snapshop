#include <gtk-3.0/gtk/gtk.h>
#include <err.h>
#include "image_process.h"

#ifndef SNAPSHOP_IMAGE_BINARIZE_H
#define SNAPSHOP_IMAGE_BINARIZE_H

void binarize(GdkPixbuf *image, int tresh);

#endif
