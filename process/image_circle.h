#include <gtk-3.0/gtk/gtk.h>
#include <err.h>
#include "image_process.h"

#ifndef SNAPSHOP_IMAGE_CIRCLE_H
#define SNAPSHOP_IMAGE_CIRCLE_H

void Fill_circle
(GdkPixbuf *image, struct color *color, int x_center, int y_center, int r);
void Draw_circle
(GdkPixbuf *image, struct color *color, int x_center, int y_center, int r);
void testCircleDraw
(GdkPixbuf *image, struct color *color, int xc, int yc, int r, int filled);
void fill
(GdkPixbuf *image,int xc, int yc, int x, int y, struct color *color);
void draw
(GdkPixbuf *image,int xc, int yc, int x, int y, struct color *color);
#endif
