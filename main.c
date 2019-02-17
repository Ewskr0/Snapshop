#include <stdlib.h>
#include <stdio.h>
#include <gtk-3.0/gtk/gtk.h>
#include <string.h>

#include "io/io.h"

#include "process/image_process.h"
#include "process/image_circle.h"
#include "process/image_scale.h"
#include "process/image_grayscale.h"


int test_scale(char *path)
{
  printf(" ==== Testing image_scale method ====\n");
  GdkPixbuf *image = Load_image(path);

  GdkPixbuf *scale_1;
  scale_1 = Scale_neighbors(image, 300, 300);
  Save_pixbuf("img_out/img_rescaled.png", "png", scale_1);
  printf("- Scale_neighbors x300 \n");

  return 0;
}

int test_drawcircle(char *path, int r)
{
  struct color *Couleur = malloc(sizeof(struct color));
  Couleur->red = 255; Couleur->green = 255; Couleur->blue = 255;
  Couleur->opacity = 255;
  printf(" === Testing Draw_circle method ====\n");
  GdkPixbuf *image = Load_image(path);
  testCircleDraw(image, Couleur, 150, 150, r, 0);
  Save_pixbuf("img_out/img_drawcircle.png", "png", image);
  printf("- Circle drawn (150, 150)\n");

  return 0;    
}

int test_fillcircle(char *path, int r)
{
  struct color *Couleur = malloc(sizeof(struct color));
  Couleur->red = 255; Couleur->green = 255; Couleur->blue = 255; Couleur->opacity =
    255;
  printf(" === Testing fill_circle method ====\n");
  GdkPixbuf *image = Load_image(path);
  testCircleDraw(image, Couleur, 150, 150, r, 1);
  Save_pixbuf("img_out/img_fillcircle.png", "png", image);
  printf("- Circle filled (150, 150)\n");

  return 0; 
}

int test_grayscale(char *path)
{  
  printf(" ==== Testing image_grayscale method ====\n"); 
  GdkPixbuf *image = Load_image(path);
  GrayScale(image);
  Save_pixbuf("img_out/img_grayscale.png", "png", image);
  
  printf(" ==== Testing image_binarize method ====\n");
  Binarize(image, 127);
  Save_pixbuf("img_out/img_binarized.png", "png", image);
  printf("- Image converted in gray and binarized\n");

  return 0;
}

int main(int argc, char *argv[])
{

  if (argc != 3)
    return 0;
  int r = atoi(argv[2]);
  test_scale(argv[1]);
  test_drawcircle(argv[1], r);
  test_fillcircle(argv[1], r);
  test_grayscale(argv[1]);
  printf(" ==== Results in img_out/ ==== \n");

  return 0;
}

