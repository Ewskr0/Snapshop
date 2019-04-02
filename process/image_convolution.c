#include "image_convolution.h"


guchar Truncate_256(int value)
{
  if(value > 255)
    return (guchar)255;
  if(value < 0)
    return (guchar)0;
  return (guchar)value;
}

guchar Truncate_256_double(double value)
{
  if(value > 255)
    return (guchar)255;
  if(value < 0)
    return (guchar)0;
  return (guchar)value;
}

GdkPixbuf* Apply_matrix(GdkPixbuf* image, struct convolution_matrix* matrix)
{
  int width = gdk_pixbuf_get_width(image);
  int height = gdk_pixbuf_get_height(image);
  int offset = matrix->size / 2;
  GdkPixbuf* new = gdk_pixbuf_copy(image);

  // For each pixel of the image where the matrix can be applied
  for(int y = offset; y < height - offset; ++y)
  {
    for(int x = offset; x < width - offset; ++x)
    {
      int r_canal = 0;
      int g_canal = 0;
      int b_canal = 0;

      guchar* applied_pixel = Get_pixel(new, x, y);

      for(int i = 0; i < matrix->size; ++i)
      {
        for (int j = 0; j < matrix->size; ++j) {
          guchar* cur_pixel = Get_pixel(image, x + j - offset, y + i - offset);
          int cur_coeff = matrix->matrix[i * matrix->size + j];
          r_canal += *(cur_pixel) * cur_coeff;
          g_canal += *(cur_pixel + 1) * cur_coeff;
          b_canal += *(cur_pixel + 2) * cur_coeff;
        }
      }
      r_canal = r_canal / matrix->divisor;
      g_canal = g_canal / matrix->divisor;
      b_canal = b_canal / matrix->divisor;

      *(applied_pixel)     = Truncate_256(r_canal);
      *(applied_pixel + 1) = Truncate_256(g_canal);
      *(applied_pixel + 2) = Truncate_256(b_canal);
    }
  }
  return new;
}

struct convolution_matrix* InitMotionBlur(int size, int val) {

  if (size % 2 == 0)
    fprintf(stderr, "Need a odd matrix's size");
  int NbElt = size * size;
  struct convolution_matrix *mat = calloc(1, sizeof(struct convolution_matrix));
  mat->size = size;
  int *GaussianFilter = calloc(NbElt, sizeof(int));
  mat->matrix = GaussianFilter;
  mat->divisor = size;
  if (val == 1) { //Corner up left to down right
    int pl = 0;
    int i = 0;
    //GaussianFilter[0] = 1;
    while (i < size * size) {
      //int val = i%size;
      //printf("i = %i \n", i);
      if (i % size == pl) {
        GaussianFilter[i] = 1;
        pl++;
        i++;
        while (i % size != 0) {
          GaussianFilter[i] = 0;
          i++;
        }
      } else {
        GaussianFilter[i] = 0;
        i++;
      }
    }
  } else {
    if (val == 2) { //corner down left to up right
      int pl = size - 1;
      int i = 0;
      //GaussianFilter[0] = 1;
      while (i < size * size) {
        //int val = i%size;
        //printf("i = %i \n", i);
        if (i % size == pl) {
          GaussianFilter[i] = 1;
          pl--;
          i++;
          while (i % size != 0) {
            GaussianFilter[i] = 0;
            i++;
          }
        } else {
          GaussianFilter[i] = 0;
          i++;
        }
      }
    } else {
      if (val == 3) { //horizontal
        int pl = size / 2;
        for (int i = 0; i < size * size; i++) {
          if (i % size == pl)
            GaussianFilter[i] = 1;
          else
            GaussianFilter[i] = 0;
        }
      } else {
        if (val == 4) { //vertical
          int i = 0;
          while (i < (size * size / 2) - size / 2) {
            GaussianFilter[i] = 0;
            i++;
            GaussianFilter[size * size - i] = 0;
          }
          int j = i + size;
          while (i < j) {
            GaussianFilter[i] = 1;
            i++;
          }
        }
      }
    }
  }
  return mat;
}

GdkPixbuf* Motion_blur_hor(GdkPixbuf* image, int size) {
  GdkPixbuf *new = gdk_pixbuf_copy(image);

  for (int y = 0; y < gdk_pixbuf_get_height(image); ++y) {
    for (int x = size / 2; x < gdk_pixbuf_get_width(image) - size/2 ; ++x) {
      int red = 0;
      int green = 0;
      int blue = 0;
      for (int i = 0; i < size; ++i) {
        guchar* cur = Get_pixel(image, x - size/2 + i, y);
        red += cur[0];
        green += cur[1];
        blue += cur[2];
      }
      guchar* cur = Get_pixel(new, x, y);
      cur[0] = Truncate_256(red / size);
      cur[1] = Truncate_256(green / size);
      cur[2] = Truncate_256(blue /size);
    }
  }
  return new;
}
