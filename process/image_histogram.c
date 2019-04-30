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

  for (int i = 0; i < 256; ++i)
  {
    // graphBlack
    struct box black_box = {i * 2, 245 - (histo->light[i] * 100 / correction_factor) * 2, i * 2, 245};
    Fill_color(graphBlack, &black_color, &black_box);

    // graphRed
    struct box red_box = {i * 2, 245 - (histo->red[i] * 100 / correction_factor) * 2, i * 2, 245};
    Fill_color(graphRed, &red_color, &red_box);

    // graphGreen
    struct box green_box = {i * 2, 245 - (histo->green[i] * 100 / correction_factor) * 2, i * 2, 245};
    Fill_color(graphGreen, &green_color, &green_box);

    // graphBLue
    struct box blue_box = {i * 2, 245 - (histo->blue[i] * 100 / correction_factor) * 2, i * 2, 245};
    Fill_color(graphBLue, &blue_color, &blue_box);

    // graphAll
    struct box black_boxAll = {i * 2, 245 - (histo->light[i] * 100 / correction_factor) * 2, i * 2, 245 - (histo->light[i] * 100 / correction_factor) * 2};
    Fill_color(graphAll, &black_color, &black_boxAll);
    struct box red_boxAll = {i * 2, 245 - (histo->red[i] * 100 / correction_factor) * 2, i * 2, 245 - (histo->red[i] * 100 / correction_factor) * 2};
    Fill_color(graphAll, &red_color, &red_boxAll);
    struct box green_boxAll = {i * 2, 245 - (histo->green[i] * 100 / correction_factor) * 2, i * 2, 245 - (histo->green[i] * 100 / correction_factor) * 2};
    Fill_color(graphAll, &green_color, &green_boxAll);
    struct box blue_boxAll = {i * 2, 245 - (histo->blue[i] * 100 / correction_factor) * 2, i * 2, 245 - (histo->blue[i] * 100 / correction_factor) * 2};
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

void GreyNormalize(GdkPixbuf *image){
  int width = gdk_pixbuf_get_width(image);
  int height = gdk_pixbuf_get_height(image);

  int min = 255, max = 0;
  double scale;

  struct histogram *histo = Histogram(image);


  for(int i = 0; p < 256; p++) {
    if(histo->light[i] <  min) min = i;
    if(histo->light[i] >  max) max = i;
  }
  
  scale = 255.0 / (max-min);

  for(int y = 0; y <  height; y++)
  {
    for(int x = 0; x < width; x++)
    {
      guchar* cur_pixel = Get_pixel(image, x, y);
      int red = cur_pixel[0];

      int gray = red;
      gray -= min;
      gray *= scale;

      cur_pixel[0] = gray;
      cur_pixel[1] = gray;
      cur_pixel[2] = gray;
    }
  }
}
