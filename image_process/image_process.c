#include "image_process.h"
#include "convolution.h"


guchar* Get_pixel(GdkPixbuf* image, int x, int y)
{
  // Error // We are in base 0
  if(y >= gdk_pixbuf_get_height(image))
    err(0, "Error in Get_pixel() : the y is greater than the height");
  if(x >= gdk_pixbuf_get_width(image))
    err(0, "Error in Get_pixel() : the x is greater than the width");

  return gdk_pixbuf_get_pixels(image) + y * gdk_pixbuf_get_rowstride(image)
         + gdk_pixbuf_get_n_channels(image) * x;
}

guchar* Get_pixel_out(GdkPixbuf* image, int x, int y)
{
  if(x < 0)
    x = 0;
  if(y < 0)
    y = 0;
  if(x >= gdk_pixbuf_get_width(image))
    x = gdk_pixbuf_get_width(image) - 1;
  if(y >= gdk_pixbuf_get_height(image))
    y = gdk_pixbuf_get_height(image);
  return gdk_pixbuf_get_pixels(image) + y * gdk_pixbuf_get_rowstride(image)
         + 3 * x;
}

void Fill_color(GdkPixbuf *image, struct color *color, struct box *box)
{
  int x1 = box->x1 > 0 ? box->x1 : 0;
  int x2 = box->x2 < gdk_pixbuf_get_width(image) ?
           box->x2 : gdk_pixbuf_get_width(image);
  int y1 = box->y1 > 0 ? box->y1 : 0;
  int y2 = box->y2 < gdk_pixbuf_get_height(image) ?
           box->y2 : gdk_pixbuf_get_height(image);

  for(int y = y1; y <= y2; ++y)
    for (int x = x1; x <= x2; ++x) {
      guchar* p = Get_pixel_out(image, x, y);
      *(p)   = (*p * ((guchar)100 - color->opacity) +
              (color->opacity * color->red))/(guchar)100;
      *(p + 1) = (*(p + 1) * ((guchar)100 - color->opacity) +
              (color->opacity * color->green))/(guchar)100;
      *(p + 2) = (*(p + 2) * ((guchar)100 - color->opacity) +
              (color->opacity * color->blue))/(guchar)100;
    }
}

void Color_pixel(guchar* p, struct color *color)
{
  *(p)   = (*p * ((guchar)100 - color->opacity) +
            (color->opacity * color->red))/(guchar)100;
  *(p + 1) = (*(p + 1) * ((guchar)100 - color->opacity) +
              (color->opacity * color->green))/(guchar)100;
  *(p + 2) = (*(p + 2) * ((guchar)100 - color->opacity) +
              (color->opacity * color->blue))/(guchar)100;
}

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

GdkPixbuf* Scale_neighbors(GdkPixbuf *image, int x_scale, int y_scale)
{
  int height = (y_scale * gdk_pixbuf_get_height(image))/100;
  int width = (x_scale * gdk_pixbuf_get_width(image))/100;

  GdkPixbuf* new_image = gdk_pixbuf_new(GDK_COLORSPACE_RGB,0,8,width,height);
  for(int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x) {
      guchar* current_new = Get_pixel(new_image, x, y);
      guchar* current_old = Get_pixel_out(image, (x * 100) / x_scale, (y * 100)/y_scale);
      for (int i = 0; i < gdk_pixbuf_get_n_channels(image); ++i)
        *(current_new + i) = *(current_old + i);
    }
  }
  return new_image;
}

GdkPixbuf* Scale_bilinear(GdkPixbuf *image, int x_scale, int y_scale)
{
  int height = (y_scale * gdk_pixbuf_get_height(image))/100;
  int width = (x_scale * gdk_pixbuf_get_width(image))/100;

  GdkPixbuf* new_image = gdk_pixbuf_new(GDK_COLORSPACE_RGB,0,8,width,height);
  for(int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x) {
      guchar* current_new = Get_pixel(new_image, x, y);

      // Where the original pixel we want to copy SHOULD be (is a floating point number)
      double theory_position_x = ((double)x * 100.0) / (double)x_scale;
      double theory_position_y = ((double)y * 100.0) / (double)y_scale;

      int pos_x_aligned = (x * 100) / x_scale;
      int pos_y_aligned = (y * 100) / y_scale;

      guchar *i_00, *i_01, *i_10, *i_11;

      // Get the for pixels on the original image that surround the theorical
      // Position of the computed pixel

      i_00 = Get_pixel_out(image, pos_x_aligned, pos_y_aligned);
      i_01 = Get_pixel_out(image, pos_x_aligned + 1, pos_y_aligned);
      i_10 = Get_pixel_out(image, pos_x_aligned, pos_y_aligned + 1);
      i_11 = Get_pixel_out(image, pos_x_aligned + 1, pos_y_aligned + 1);

      for (int i = 0; i < gdk_pixbuf_get_n_channels(image); ++i)
      {
        // X interpolation:
        double value_x1 = ((pos_x_aligned + 1) - theory_position_x) * i_00[i]
                + (theory_position_x - pos_x_aligned) * i_01[i];

        double value_x2 = ((pos_x_aligned + 1) - theory_position_x) * i_10[i]
                + (theory_position_x - pos_x_aligned) * i_11[i];

        *(current_new + i) = Truncate_256_double(
                ((pos_y_aligned + 1) - theory_position_y) * value_x1
                + (theory_position_y - pos_y_aligned) * value_x2);
      }
    }
  }
  return new_image;
}

GdkPixbuf* Truncate(GdkPixbuf* image, struct box* box)
{
  int height = box->y2 - box->y1;
  int width = box->x2 - box->y1;
  if(height < 0 || width < 0)
    err(EXIT_FAILURE, "Error in the size (< 0) in truncate function");
  if(box->x2 >= gdk_pixbuf_get_width(image) || box->y2 >= gdk_pixbuf_get_height(image))
    err(EXIT_FAILURE, "Error in the size, the box exceed the size of the image");
  GdkPixbuf* new_image = gdk_pixbuf_new(GDK_COLORSPACE_RGB,0,8,width,height);
  for(int y = 0; y < width; ++y)
  {
    for (int x = 0; x < width; ++x) {
      guchar* current = Get_pixel(new_image,x,y);
      guchar* old = Get_pixel(image, x + box->x1, y + box->y1);
      for (int i = 0; i < gdk_pixbuf_get_n_channels(image); ++i) {
        *(current + i) = *(old + i);
      }
    }
  }
  return new_image;
}