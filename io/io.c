#include "io.h"

GdkPixbuf* Load_image(char* path) {
  GdkPixbuf *image = gdk_pixbuf_new_from_file(path, NULL);
  if(!image)
    err(1, "Could not load the image.");
  return image;
}

void Save_pixbuf(char* path, char* type, GdkPixbuf* image)
{
  if(!gdk_pixbuf_save(image, path, type, NULL, NULL))
    err(0, "Could not save the file");
}
