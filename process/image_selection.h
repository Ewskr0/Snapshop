#ifndef SNAPSHOP_SELECTION_H
#define SNAPSHOP_SELECTION_H

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdio.h>
#include <stdlib.h>
#include "image_process.h"

struct selection
{
  int height, width;
  GdkPixbuf *image;
  char *matrix;
};

struct selection *Selection_init(GdkPixbuf *image);
void Selection_free(struct selection *selection);
void Selection_circle(struct selection *sel, int x_center, int y_center, int r, char bool);
void Selection_box(struct selection *sel, struct box *box, char bool);
void Selection_color(struct selection *sel, struct color *color, float dst, char bool);

#endif