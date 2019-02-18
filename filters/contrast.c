#include "contrast.h"

void Brightness(GdkPixbuf *image, int offset)
{
  for (int y = 0; y < gdk_pixbuf_get_height(image); ++y) {
    for (int x = 0; x < gdk_pixbuf_get_width(image); ++x) {
      guchar* cur = Get_pixel(image, x, y);
      *(cur) = Truncate_256(*cur + offset);
      *(cur + 1) = Truncate_256(*(cur + 1) + offset);
      *(cur + 2) = Truncate_256(*(cur + 2) + offset);
    }
  }
}

void Contrast(GdkPixbuf *image, int offset)
{
  double factor = (259 * ((double)offset + 255)/(255 * (259 - (double)offset)));
  for (int y = 0; y < gdk_pixbuf_get_height(image); ++y) {
    for (int x = 0; x < gdk_pixbuf_get_width(image); ++x) {
      guchar* cur = Get_pixel(image, x, y);
      *(cur) = Truncate_256_double(factor * (*cur - 128) + 128);
      *(cur + 1) = Truncate_256_double(factor * (*(cur + 1) - 128) + 128);
      *(cur + 2) = Truncate_256_double(factor * (*(cur + 2) - 128) + 128);
    }
  }
}

void Gradient_map(GdkPixbuf *image, struct color *dark, struct color *bright)
{
  for (int y = 0; y < gdk_pixbuf_get_height(image); ++y) {
    for (int x = 0; x < gdk_pixbuf_get_width(image); ++x) {
      guchar* cur = Get_pixel(image, x, y);
      int brightness = (cur[0] + cur[1] + cur[2]) / 3;
      cur[0] = Truncate_256_double
              (brightness * (bright->red - dark->red)/255 + dark->red);
      cur[1] = Truncate_256_double
              (brightness * (bright->green - dark->green)/255 + dark->green);
      cur[2] = Truncate_256_double
              (brightness * (bright->blue - dark->blue)/255 + dark->blue);
    }
  }

}
