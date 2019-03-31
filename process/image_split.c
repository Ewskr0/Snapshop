#include "image_split.h"

GdkPixbuf** Split(GdkPixbuf* image)
{
    int height = gdk_pixbuf_get_height(image);
    int width = gdk_pixbuf_get_width(image);

    GdkPixbuf** result = malloc(sizeof(image)*3);
    
    result[0] = gdk_pixbuf_new(GDK_COLORSPACE_RGB, 0, 8, width, height);
    result[1] = gdk_pixbuf_new(GDK_COLORSPACE_RGB, 0, 8, width, height);
    result[2] = gdk_pixbuf_new(GDK_COLORSPACE_RGB, 0, 8, width,
	    height); 
    for(int y = 0; y < height; y++)
    {
	for(int x = 0; x < width; x++)
	{
	    guchar* cur_pixel = Get_pixel(image, x, y);
	    guchar* red = Get_pixel(result[0], x, y);
	    guchar* green = Get_pixel(result[1], x, y);
	    guchar* blue = Get_pixel(result[2], x, y);

	    red[0] = cur_pixel[0];
	    red[1] = 0; red[2] = 0;

	    green[1] = cur_pixel[1];
	    green[0] = 0; green[2] = 0;

	    blue[2] = cur_pixel[2];
	    blue[0] = 0; blue[1] = 0;
	}
    }
    return result;
}
