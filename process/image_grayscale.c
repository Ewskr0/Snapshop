#include "image_grayscale.h"

void GrayScale(GdkPixbuf *image){
  int width = gdk_pixbuf_get_width(image);
  int height = gdk_pixbuf_get_height(image);

  for(int y = 0; y <  height; y++)
  {
    for(int x = 0; x < width; x++)
    {
      guchar* cur_pixel = Get_pixel(image, x, y);
      int red = cur_pixel[0];
      int green = cur_pixel[1];
      int blue = cur_pixel[2];

      int gray = (int)(red * 0.21 + green * 0.72
	  + blue * 0.07);

      cur_pixel[0] = gray;
      cur_pixel[1] = gray;
      cur_pixel[2] = gray;
    }
  }
}
