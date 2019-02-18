#include <gtk-3.0/gtk/gtk.h>
#include <err.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef SNAPSHOP_CONTRAST_H
#define SNAPSHOP_CONTRAST_H

void Brighthness(GdkPixbuf *image, int offset);
void Contrast(GdkPixbuf *image, int offset);
void Gradient_map(GdkPixbuf *image, struct color *dark, struct color *bright);
