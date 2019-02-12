#include <stdlib.h>
#include <stdio.h>
#include <gtk-3.0/gtk/gtk.h>
#include <string.h>

#include "io/io.h"

#include "process/image_process.h"
#include "process/image_circle.h"
#include "process/image_scale.h"

int main(int argc, char *argv[])
{

  if (argc != 2)
    return 0;
  GdkPixbuf *image = Load_image(argv[1]);

  GdkPixbuf *scale_1;
  scale_1 = Scale_neighbors(image, 300, 300);
  Save_pixbuf("img_out/13_Scale_01.png", "png", scale_1);

  free(image);
  free(scale_1);
  
  return 0;
}