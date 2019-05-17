#include "../io/io.h"
#include "image_process.h"

#ifndef SNAPSHOP_TOOLS_H
#define SNAPSHOP_TOOLS_H

struct histogram {
  int light[256];
  int red[256];
  int green[256];
  int blue[256];
};

struct histogram *Histogram(GdkPixbuf *image);
GdkPixbuf *Create_histo_graph(GdkPixbuf *image);
void HistogramNormalize(GdkPixbuf *image);
void HistogramEqualisation(GdkPixbuf *image);
#endif