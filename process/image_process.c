#include "image_process.h"
#include "image_convolution.h"


guchar* Get_pixel(GdkPixbuf* image, int x, int y)
{
    // Error // We are in base 0
    if(y >= gdk_pixbuf_get_height(image))
	err(0, "Error in Get_pixel() : the y is greater than the height");
    if(x >= gdk_pixbuf_get_width(image))
	err(0, "Error in Get_pixel() : the x is greater than the width");

    return gdk_pixbuf_get_pixels(image) + y * gdk_pixbuf_get_rowstride(image)
	+ gdk_pixbuf_get_n_channels(image) * x;
}

guchar* Get_pixel_out(GdkPixbuf* image, int x, int y)
{
    if(x < 0)
	x = 0;
    if(y < 0)
	y = 0;
    if(x >= gdk_pixbuf_get_width(image))
	x = gdk_pixbuf_get_width(image) - 1;
    if(y >= gdk_pixbuf_get_height(image))
	y = gdk_pixbuf_get_height(image);
    return gdk_pixbuf_get_pixels(image) + y * gdk_pixbuf_get_rowstride(image)
	+ 3 * x;
}

void Fill_color(GdkPixbuf *image, struct color *color, struct box *box)
{
    int x1 = box->x1 > 0 ? box->x1 : 0;
    int x2 = box->x2 < gdk_pixbuf_get_width(image) ?
	box->x2 : gdk_pixbuf_get_width(image);
    int y1 = box->y1 > 0 ? box->y1 : 0;
    int y2 = box->y2 < gdk_pixbuf_get_height(image) ?
	box->y2 : gdk_pixbuf_get_height(image);

    for(int y = y1; y <= y2; ++y)
	for (int x = x1; x <= x2; ++x) {
	    guchar* p = Get_pixel_out(image, x, y);
	    *(p)   = (*p * ((guchar)100 - color->opacity) +
		    (color->opacity * color->red))/(guchar)100;
	    *(p + 1) = (*(p + 1) * ((guchar)100 - color->opacity) +
		    (color->opacity * color->green))/(guchar)100;
	    *(p + 2) = (*(p + 2) * ((guchar)100 - color->opacity) +
		    (color->opacity * color->blue))/(guchar)100;
	}
}

void Fill_color2(GdkPixbuf *image, struct color *color, struct box *box)
{
    int x1 = box->x1 > 0 ? box->x1 : 0;
    int x2 = box->x2 < gdk_pixbuf_get_width(image) ?
	box->x2 : gdk_pixbuf_get_width(image);
    int y1 = box->y1 > 0 ? box->y1 : 0;
    int y2 = box->y2 < gdk_pixbuf_get_height(image) ?
	box->y2 : gdk_pixbuf_get_height(image);

    for(int y = y1; y <= y2; ++y)
	for (int x = x1; x <= x2; ++x) {
	    guchar* p = Get_pixel_out(image, x, y);
	    p[0] = color->red;
	    p[1] = color->green;
	    p[2] = color->blue;
	}
}

void Draw_rect(GdkPixbuf *image, struct color *color, struct box *box)
{
   int x1 = box->x1 > 0 ? box->x1 : 0;
   int x2 = box->x2 < gdk_pixbuf_get_width(image) ?
	box->x2 : gdk_pixbuf_get_width(image);
   int y1 = box->y1 > 0 ? box->y1 : 0;
   int y2 = box->y2 < gdk_pixbuf_get_height(image) ?
	box->y2 : gdk_pixbuf_get_height(image);

   int y = y1;
   for (int x = x1; x <= x2; ++x) {
	    guchar* p = Get_pixel_out(image, x, y);
	    p[0] = color->red;
	    p[1] = color->green;
	    p[2] = color->blue;
	
   }
   y = y2;
   for (int x = x1; x <= x2; ++x) {
	    guchar* p = Get_pixel_out(image, x, y);
	    p[0] = color->red;
	    p[1] = color->green;
	    p[2] = color->blue;
   }
   int x=x1;
   for (int j = y1; j <= y2; ++j) {
	    guchar* p = Get_pixel_out(image, x, j);
	    p[0] = color->red;
	    p[1] = color->green;
	    p[2] = color->blue;
   }
   x=x2;
   for (int j = y1; j <= y2; ++j) {
	    guchar* p = Get_pixel_out(image, x, j);
	    p[0] = color->red;
	    p[1] = color->green;
	    p[2] = color->blue;
   }
}

void Color_pixel(guchar* p, struct color *color)
{
    /* *(p)   = ((guchar)100 - color->opacity) +
	(color->opacity * color->red)/(guchar)100;
    *(p + 1) = ((guchar)100 - color->opacity) +
	(color->opacity * color->green)/(guchar)100;
    *(p + 2) = ((guchar)100 - color->opacity) +
	(color->opacity * color->blue)/(guchar)100;
	*/
	p[0] = color->red;
	p[1] = color->green;
	p[2] = color->blue;
}
