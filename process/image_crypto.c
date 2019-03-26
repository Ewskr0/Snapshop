#include "image_crypto.h"

GdkPixbuf* Crypto(GdkPixbuf *image){

    int height = gdk_pixbuf_get_height(image);
    int width = gdk_pixbuf_get_width(image);
    int code;
    int old;
    GdkPixbuf* crypto = gdk_pixbuf_new(GDK_COLORSPACE_RGB, 0, 8, width, height);
    
    
    for(int y = 0; y < height; y++)
    {
	for(int x = 0; x < width; x++)
	{
	    guchar* cur_pixel = Get_pixel(image, x, y);
	    guchar* code_pixel = Get_pixel(crypto, x, y);  

	    for(int i = 0; i < gdk_pixbuf_get_n_channels(image); i++)
	    {
		code = rand()%256;
		old = cur_pixel[i];
		code_pixel[i] = code;
		cur_pixel[i] = code ^ old;
	    }
	}
    }
    
    return crypto;
     
}

void Uncrypt(GdkPixbuf* image, GdkPixbuf* code)
{
    int height = gdk_pixbuf_get_height(image);
    int width = gdk_pixbuf_get_width(image);
    
    for(int y = 0; y < height; y++)
    {
	for(int x = 0; x < width; x++)
	{
	    
	    guchar* curr_pixel = Get_pixel(image, x, y);
	    guchar* code_pixel = Get_pixel(code, x, y); 
	    
	    int red = curr_pixel[0];
	    int green = curr_pixel[1];
	    int blue = curr_pixel[2];

	    int codeRed = code_pixel[0]; 
	    int codeGreen = code_pixel[1]; 
	    int codeBlue = code_pixel[2];
	   
	    
	    int newRed = red ^ codeRed;
	    int newGreen = green ^ codeGreen;
	    int newBlue = blue ^ codeBlue;

	    
	    curr_pixel[0] = newRed;
	    curr_pixel[1] = newGreen;
	    curr_pixel[2] = newBlue;

	
	}
    }
}
