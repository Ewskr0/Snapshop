#include <gtk-3.0/gtk/gtk.h>
#include <err.h>

#ifndef SNAPSHOP_IMAGE_PROCESS_H
#define SNAPSHOP_IMAGE_PROCESS_H

struct color{
    guchar red, green, blue;
    guchar opacity;
};

struct box{
    int x1, y1, x2, y2;
};

guchar* Get_pixel(GdkPixbuf* image, int x, int y);
void Fill_color(GdkPixbuf *image, struct color *color, struct box *box);
void Color_pixel(guchar* pixel, struct color *color);

#endif