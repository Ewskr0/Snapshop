#include <gtk-3.0/gtk/gtk.h>
#include <err.h>
#include <time.h>
#include "image_process.h"

#ifndef SNAPSHOP_IMAGE_CRYPTO_H
#define SNAPSHOP_IMAGE_CRYPTO_H
GdkPixbuf* Crypto(GdkPixbuf *image);
void Uncrypt(GdkPixbuf* image, GdkPixbuf* code);
#endif
