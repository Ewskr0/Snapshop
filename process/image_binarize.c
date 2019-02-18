#include "image_binarize.h"

void binarize(GdkPixbuf *image, int tresh)
{
  if(tresh < 0 || tresh > 255)
    errx(EXIT_FAILURE, "tresh must be between 0 and 255");

  int width = gdk_pixbuf_get_width(image);
  int height = gdk_pixbuf_get_height(image);

  for(int y = 0; y < height; y++)
  {
    for(int x = 0; x < width; x++)
    {
      guchar* cur_pixel = Get_pixel(image,x ,y);
      int gray = cur_pixel[0];

      if(gray <= tresh)
      {
	cur_pixel[0] = 0;
	cur_pixel[1] = 0;
	cur_pixel[2] = 0;
      }
      else
      {
	cur_pixel[0] = 255;
	cur_pixel[1] = 255;
	cur_pixel[2] = 255;
      }

    }
  }
}
