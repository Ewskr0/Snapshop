#include "image_histogram.h"

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
  GdkPixbuf *graphBlack = Load_image("./src/image/emptyGraph.jpg");
  GdkPixbuf *graphRed = Load_image("./src/image/emptyGraph.jpg");
  GdkPixbuf *graphGreen = Load_image("./src/image/emptyGraph.jpg");
  GdkPixbuf *graphBLue = Load_image("./src/image/emptyGraph.jpg");
  GdkPixbuf *graphAll = Load_image("./src/image/emptyGraph.jpg");

  int correction_factor = Get_max_histo(histo);
  struct color red_color = {255, 0, 0, 100};
  struct color green_color = {0, 255, 0, 100};
  struct color blue_color = {0, 0, 255, 100};
  struct color black_color = {0, 0, 0, 100};

  for (int i = 0; i < 256; ++i)
  {
    // graphBlack
    struct box black_box = {i * 2, 200 - (histo->light[i] * 100 / correction_factor) * 2, i * 2, 200};
    Fill_color(graphBlack, &black_color, &black_box);

    // graphRed
    struct box red_box = {i * 2, 200 - (histo->red[i] * 100 / correction_factor) * 2, i * 2, 200};
    Fill_color(graphRed, &red_color, &red_box);

    // graphGreen
    struct box green_box = {i * 2, 200 - (histo->green[i] * 100 / correction_factor) * 2, i * 2, 200};
    Fill_color(graphGreen, &green_color, &green_box);

    // graphBLue
    struct box blue_box = {i * 2, 200 - (histo->blue[i] * 100 / correction_factor) * 2, i * 2, 200};
    Fill_color(graphBLue, &blue_color, &blue_box);

    // graphAll
    struct box black_boxAll = {i * 2, 200 - (histo->light[i] * 100 / correction_factor) * 2, i * 2, 200 - (histo->light[i] * 100 / correction_factor) * 2};
    Fill_color(graphAll, &black_color, &black_boxAll);
    struct box red_boxAll = {i * 2, 200 - (histo->red[i] * 100 / correction_factor) * 2, i * 2, 200 - (histo->red[i] * 100 / correction_factor) * 2};
    Fill_color(graphAll, &red_color, &red_boxAll);
    struct box green_boxAll = {i * 2, 200 - (histo->green[i] * 100 / correction_factor) * 2, i * 2, 200 - (histo->green[i] * 100 / correction_factor) * 2};
    Fill_color(graphAll, &green_color, &green_boxAll);
    struct box blue_boxAll = {i * 2, 200 - (histo->blue[i] * 100 / correction_factor) * 2, i * 2, 200 - (histo->blue[i] * 100 / correction_factor) * 2};
    Fill_color(graphAll, &blue_color, &blue_boxAll);
  }
  Save_pixbuf("./img_out/graphBlack.png", "png", graphBlack);
  Save_pixbuf("./img_out/graphRed.png", "png", graphRed);
  Save_pixbuf("./img_out/graphGreen.png", "png", graphGreen);
  Save_pixbuf("./img_out/graphBLue.png", "png", graphBLue);
  Save_pixbuf("img_out/graphBlack.png", "png", graphBlack);
  Save_pixbuf("img_out/graphAll.png", "png", graphAll);

  return graphAll;
}
