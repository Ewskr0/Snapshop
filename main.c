#include <stdlib.h>
#include <stdio.h>
#include <gtk-3.0/gtk/gtk.h>
#include <string.h>

#include "io/io.h"

#include "process/image_process.h"
#include "process/image_circle.h"
#include "process/image_scale.h"


int test_scale(char *path)
{
  printf(" ==== Testing image_scale methods ====\n");
  GdkPixbuf *image = Load_image(path);

  GdkPixbuf *scale_1;
  scale_1 = Scale_neighbors(image, 300, 300);
  Save_pixbuf("img_out/13_Scale_01.png", "png", scale_1);
  printf("- Scale_neighbors x300 \n");

  return 0;
}

int main(int argc, char *argv[])
{

  if (argc != 2)
    return 0;
  test_scale(argv[1]);

  printf(" ==== Results in img_out/ ==== \n");

  return 0;
}

