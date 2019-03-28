#include "image_reverse.h"

void ReverseVert(GdkPixbuf *image)
{
    int width = gdk_pixbuf_get_width(image);
    int height = gdk_pixbuf_get_height(image);
    int tmp = 0;
    for(int y = 0; y < height/2; y++)
    {
	for(int x = 0; x < width; x++)
	{
	    guchar* cur_pixel = Get_pixel(image, x, y);
	    guchar* mirror_pixel = Get_pixel(image, x, height -y -1);
	    for(int n = 0; n < gdk_pixbuf_get_n_channels(image); n++)
	    {
		tmp = cur_pixel[n];

		cur_pixel[n] = mirror_pixel[n];
		mirror_pixel[n] = tmp;
	    }
	}
    }
}

void ReverseHor(GdkPixbuf *image)
{
    int width = gdk_pixbuf_get_width(image);
    int height = gdk_pixbuf_get_height(image);
    int tmp = 0;
    for(int y = 0; y < height; y++)
    {
	for(int x = 0; x < width/2; x++)
	{
	    guchar* cur_pixel = Get_pixel(image, x, y);
	    guchar* mirror_pixel = Get_pixel(image, width -x -1, y);
	    for(int n = 0; n < gdk_pixbuf_get_n_channels(image); n++)
	    {
		tmp = cur_pixel[n];

		cur_pixel[n] = mirror_pixel[n];
		mirror_pixel[n] = tmp;
	    }
	}
    }
}
