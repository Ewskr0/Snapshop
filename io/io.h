#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <gtk-3.0/gtk/gtk.h>
#include <err.h>

#ifndef SNAPSHOP_IO_H
#define SNAPSHOP_IO_H

GdkPixbuf* Load_image(char* path);
void Save_pixbuf(char* path, char* type, GdkPixbuf* pixbuf);
void Check_dir(char* path);

#endif