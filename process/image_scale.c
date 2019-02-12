#include "image_scale.h"

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