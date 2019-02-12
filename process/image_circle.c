#include "image_circle.h"

void Draw_circle
    (GdkPixbuf *image, struct color *color, int x_center, int y_center, int r)
{
  if (r < 0)
    return;
  int x = 0;
  int y = r;
  int d = r - 1;
  while(x <= y)
  {
    Color_pixel(Get_pixel_out(image, x_center + x, y_center + y), color);
    Color_pixel(Get_pixel_out(image, x_center + y, y_center + x), color);
    Color_pixel(Get_pixel_out(image, x_center - x, y_center + y), color);
    Color_pixel(Get_pixel_out(image, x_center - y, y_center + x), color);
    Color_pixel(Get_pixel_out(image, x_center + x, y_center - y), color);
    Color_pixel(Get_pixel_out(image, x_center + y, y_center - x), color);
    Color_pixel(Get_pixel_out(image, x_center - x, y_center - y), color);
    Color_pixel(Get_pixel_out(image, x_center - y, y_center - x), color);
    if(d >= 2 * x)
    {
      d = d - 2 * x - 1;
      ++x;
    }
    else if (d < 2 * (r - y))
    {
      d = d + 2 * y - 1;
      --y;
    }
    else
    {
     d = d + 2 *(y-x-1);
      --y;
      ++x;
    }
  }
}

void Fill_circle
    (GdkPixbuf *image, struct color *color, int x_center, int y_center, int r)
{
  for(int i = 0; i <= r; ++i)
    Draw_circle(image, color, x_center, y_center, i);
}