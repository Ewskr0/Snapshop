#include "image_rotate.h"
#include "image_reverse.h"
GdkPixbuf* Rotate(GdkPixbuf *image, int rot)
{
    int width = gdk_pixbuf_get_width(image);
    int height = gdk_pixbuf_get_height(image);
    int nr = rot%4;

    GdkPixbuf* new_image = gdk_pixbuf_new(GDK_COLORSPACE_RGB, 0, 8, height, width);

    if(nr == 0)
	return image;
    else if(nr == 2)
    {
	ReverseVert(image);
	ReverseHor(image);
	return image;
    }
    else
    {
	for(int y = 0; y < height; y++)
	{
	    for(int x = 0; x < width; x++)
	    {
		guchar* cur_pixel = Get_pixel(image, x, y);
		guchar* new_pixel = Get_pixel(new_image, height -y -1, x);

		for(int n = 0; n < gdk_pixbuf_get_n_channels(image); n++)
		{
		    new_pixel[n] = cur_pixel[n];
		}
	    }
	}
	if(nr == 3)
	{
	    ReverseVert(new_image);
	    ReverseVert(new_image);
	}
	return new_image;
    }
}
