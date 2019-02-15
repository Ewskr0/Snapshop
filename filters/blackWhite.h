#include <gtk-3.0/gtk/gtk.h>
#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include "../process/image_convolution.h"

#ifndef SNAPSHOP_BLACKWHITE_H
#define SNAPSHOP_BLACKWHITE_H

// Add var to choose the greyscale we want
void Greyscale(GdkPixbuf *image);

//TODO
void BlackAndWhite(GdkPixbuf *image);