#include "blackWhite.h"

void Greyscale(GdkPixbuf *image)
{
  for (int y = 0; y < gdk_pixbuf_get_height(image); ++y)
  {
    for (int x = 0; x < gdk_pixbuf_get_width(image); ++x)
    {
      guchar *pixel = Get_pixel(image, x, y);
      guchar result = Truncate_256((21 * pixel[0] + 72 * pixel[1] + 7 * pixel[2]) / 100);
      pixel[0] = result;
      pixel[1] = result;
      pixel[2] = result;
    }
  }
}

void BlackAndWhite(GdkPixbuf *image)
{
  return;
}