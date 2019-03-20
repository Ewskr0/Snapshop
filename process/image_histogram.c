#include "image_histogram.h"
#include "../io/io.h"

struct histogram *Histogram(GdkPixbuf *image)
{
  int width = gdk_pixbuf_get_width(image);
  int height = gdk_pixbuf_get_height(image);
  struct histogram *h = calloc(1, sizeof(struct histogram));

  int *light = h->light;
  int *red = h->red;
  int *green = h->green;
  int *blue = h->blue;

  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      guchar *pixel = Get_pixel(image, x, y);
      light[(pixel[0] + pixel[1] + pixel[2]) / 3] += 1;
      red[pixel[0]] += 1;
      green[pixel[1]] += 1;
      blue[pixel[2]] += 1;
    }
  }
  return h;
}

static int Get_max_histo(struct histogram *histo)
{
  int max = 0;
  for (int i = 0; i < 256; ++i)
  {
    max = max < histo->light[i] ? histo->light[i] : max;
    max = max < histo->red[i] ? histo->red[i] : max;
    max = max < histo->green[i] ? histo->green[i] : max;
    max = max < histo->blue[i] ? histo->blue[i] : max;
  }
  return max;
}

GdkPixbuf *Create_histo_graph(GdkPixbuf *image)
{
  struct histogram *histo = Histogram(image);
  GdkPixbuf *graph = Load_image('../src/image/graph.jpg');

  int correction_factor = Get_max_histo(histo);
  struct color red_color = {255, 0, 0, 100};
  struct color green_color = {0, 255, 0, 100};
  struct color blue_color = {0, 0, 255, 100};
  struct color black_color = {0, 0, 0, 100};

  for (int i = 0; i < 256; ++i)
  {
    // Black
    struct box black_box = {83 + i, 235 - (histo->light[i] * 100 / correction_factor) * 2, 83 + i, 235};
    Fill_color(graph, &black_color, &black_box);

    // Red
    struct box red_box = {475 + i, 235 - (histo->red[i] * 100 / correction_factor) * 2, 475 + i, 235};
    Fill_color(graph, &red_color, &red_box);

    // Green
    struct box green_box = {83 + i, 525 - (histo->green[i] * 100 / correction_factor) * 2, 83 + i, 525};
    Fill_color(graph, &green_color, &green_box);
    // Blue
    struct box blue_box = {475 + i, 525 - (histo->blue[i] * 100 / correction_factor) * 2, 475 + i, 525};
    Fill_color(graph, &blue_color, &blue_box);
  }
  return graph;
}
