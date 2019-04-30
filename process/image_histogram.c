#include "image_histogram.h"
#include <math.h>

struct histogram *Histogram(GdkPixbuf *image) {
  int width = gdk_pixbuf_get_width(image);
  int height = gdk_pixbuf_get_height(image);
  struct histogram *h = calloc(1, sizeof(struct histogram));

  int *light = h->light;
  int *red = h->red;
  int *green = h->green;
  int *blue = h->blue;

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      guchar *pixel = Get_pixel(image, x, y);
      light[(pixel[0] + pixel[1] + pixel[2]) / 3] += 1;
      red[pixel[0]] += 1;
      green[pixel[1]] += 1;
      blue[pixel[2]] += 1;
    }
  }
  return h;
}

static int Get_max_histo(struct histogram *histo) {
  int max = 0;
  for (int i = 0; i < 256; ++i) {
    max = max < histo->light[i] ? histo->light[i] : max;
    max = max < histo->red[i] ? histo->red[i] : max;
    max = max < histo->green[i] ? histo->green[i] : max;
    max = max < histo->blue[i] ? histo->blue[i] : max;
  }
  return max;
}

GdkPixbuf *Create_histo_graph(GdkPixbuf *image) {
  struct histogram *histo = Histogram(image);
  GdkPixbuf *graphBlack = Load_image("./src/histogram/emptyGraph.jpg");
  GdkPixbuf *graphRed = Load_image("./src/histogram/emptyGraph.jpg");
  GdkPixbuf *graphGreen = Load_image("./src/histogram/emptyGraph.jpg");
  GdkPixbuf *graphBLue = Load_image("./src/histogram/emptyGraph.jpg");
  GdkPixbuf *graphAll = Load_image("./src/histogram/emptyGraph.jpg");

  int correction_factor = Get_max_histo(histo);
  struct color red_color = {255, 0, 0, 100};
  struct color green_color = {0, 255, 0, 100};
  struct color blue_color = {0, 0, 255, 100};
  struct color black_color = {0, 0, 0, 100};

  for (int i = 0; i < 256; ++i) {
    // graphBlack
    struct box black_box = {
        i * 2, 245 - (histo->light[i] * 100 / correction_factor) * 2, i * 2,
        245};
    Fill_color(graphBlack, &black_color, &black_box);

    // graphRed
    struct box red_box = {
        i * 2, 245 - (histo->red[i] * 100 / correction_factor) * 2, i * 2, 245};
    Fill_color(graphRed, &red_color, &red_box);

    // graphGreen
    struct box green_box = {
        i * 2, 245 - (histo->green[i] * 100 / correction_factor) * 2, i * 2,
        245};
    Fill_color(graphGreen, &green_color, &green_box);

    // graphBLue
    struct box blue_box = {i * 2,
                           245 - (histo->blue[i] * 100 / correction_factor) * 2,
                           i * 2, 245};
    Fill_color(graphBLue, &blue_color, &blue_box);

    // graphAll
    struct box black_boxAll = {
        i * 2, 245 - (histo->light[i] * 100 / correction_factor) * 2, i * 2,
        245 - (histo->light[i] * 100 / correction_factor) * 2};
    Fill_color(graphAll, &black_color, &black_boxAll);
    struct box red_boxAll = {
        i * 2, 245 - (histo->red[i] * 100 / correction_factor) * 2, i * 2,
        245 - (histo->red[i] * 100 / correction_factor) * 2};
    Fill_color(graphAll, &red_color, &red_boxAll);
    struct box green_boxAll = {
        i * 2, 245 - (histo->green[i] * 100 / correction_factor) * 2, i * 2,
        245 - (histo->green[i] * 100 / correction_factor) * 2};
    Fill_color(graphAll, &green_color, &green_boxAll);
    struct box blue_boxAll = {
        i * 2, 245 - (histo->blue[i] * 100 / correction_factor) * 2, i * 2,
        245 - (histo->blue[i] * 100 / correction_factor) * 2};
    Fill_color(graphAll, &blue_color, &blue_boxAll);
  }
  Save_pixbuf("./dst/histogram/graphBlack.png", "png", graphBlack);
  Save_pixbuf("./dst/histogram/graphRed.png", "png", graphRed);
  Save_pixbuf("./dst/histogram/graphGreen.png", "png", graphGreen);
  Save_pixbuf("./dst/histogram/graphBLue.png", "png", graphBLue);
  Save_pixbuf("./dst/histogram/graphBlack.png", "png", graphBlack);
  Save_pixbuf("./dst/histogram/graphAll.png", "png", graphAll);

  return graphAll;
}

void HistoNormalize(GdkPixbuf *image) {
  int width = gdk_pixbuf_get_width(image);
  int height = gdk_pixbuf_get_height(image);

  int minR = 25500, maxR = 0;
  int minG = 25500, maxG = 0;
  int minB = 25500, maxB = 0;

  struct histogram *histo = Histogram(image);

  for (int i = 0; i < 255; ++i) {
    if (histo->red[i] + histo->red[i + 1] <= histo->red[minR])
      minR = i;
    if (histo->red[i] > histo->red[maxR])
      maxR = i;
    if (histo->green[i] + histo->red[i + 1] <= histo->green[minG])
      minG = i;
    if (histo->green[i] > histo->green[maxG])
      maxG = i;
    if (histo->blue[i] + histo->red[i + 1] <= histo->blue[minB])
      minB = i;
    if (histo->blue[i] > histo->blue[maxB])
      maxB = i;
  }

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      guchar *cur_pixel = Get_pixel(image, x, y);
      int red = cur_pixel[0];
      int green = cur_pixel[1];
      int blue = cur_pixel[2];

      cur_pixel[0] = ((red - histo->red[minR]) * 255) /
                     (histo->red[maxR] - histo->red[minR]);
      cur_pixel[1] = ((green - histo->green[minG]) * 255) /
                     (histo->green[maxG] - histo->green[minG]);
      cur_pixel[2] = ((blue - histo->blue[minB]) * 255) /
                     (histo->blue[maxB] - histo->blue[minB]);
    }
  }
}

void HistogramEqualisation(GdkPixbuf *image) {

  int width = gdk_pixbuf_get_width(image);
  int height = gdk_pixbuf_get_height(image);
  struct histogram *histo = Histogram(image);

  // calulating total number of pixels
  int total = width * height;

  int currR = 0;
  int currG = 0;
  int currB = 0;

  int red_level[256] = {0};
  int green_level[256] = {0};
  int blue_level[256] = {0};

  for (int i = 0; i < 256; i++) {
    currR += histo->red[i];
    currG += histo->green[i];
    currB += histo->blue[i];

    red_level[i] = round((((float)currR) * 255) / total);
    green_level[i] = round((((float)currG) * 255) / total);
    blue_level[i] = round((((float)currB) * 255) / total);
  }

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {

      guchar *cur_pixel = Get_pixel(image, x, y);

      int red = cur_pixel[0];
      int green = cur_pixel[1];
      int blue = cur_pixel[2];
      cur_pixel[0] = red_level[red];
      cur_pixel[1] = green_level[green];
      cur_pixel[2] = blue_level[blue];
    }
  }
}
