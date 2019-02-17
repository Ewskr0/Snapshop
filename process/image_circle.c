#include "image_circle.h"

void testCircleDraw
(GdkPixbuf *image, struct color *color, int xc, int yc, int r, int filled)
{ 
    int x = 0, y = r; 
    int d = 3 - 2 * r;
    if(filled)
      fill(image, xc, yc, x, y, color); 
    else
      draw(image, xc, yc, x, y, color);
    while (y >= x) 
    { 
        // for each pixel we will 
        // draw all eight pixels 
          
        x++; 
  
        // check for decision parameter 
        // and correspondingly  
        // update d, x, y 
        if (d > 0) 
        { 
            y--;  
            d = d + 4 * (x - y) + 10; 
        } 
        else
            d = d + 4 * x + 6;
	if(!filled)
	  draw(image, xc, yc, x, y, color);
	else
	  fill(image, xc, yc, x, y, color);
      //  delay(50); 
    } 
    

}
void draw(GdkPixbuf *image, int xc, int yc, int x, int y, struct color *color)
{    
    Color_pixel(Get_pixel_out(image, xc + x, yc + y), color);
    Color_pixel(Get_pixel_out(image, xc + y, yc + x), color);
    Color_pixel(Get_pixel_out(image, xc - x, yc + y), color);
    Color_pixel(Get_pixel_out(image, xc - y, yc + x), color);
    Color_pixel(Get_pixel_out(image, xc + x, yc - y), color);
    Color_pixel(Get_pixel_out(image, xc + y, yc - x), color);
    Color_pixel(Get_pixel_out(image, xc - x, yc - y), color);
    Color_pixel(Get_pixel_out(image, xc - y, yc - x), color);

}

void fill(GdkPixbuf *image, int xc, int yc, int x, int y, struct color *color)
{
    for (int i = -y; i <= y; i++){
	Color_pixel(Get_pixel_out(image, xc + x, yc + i), color);
	Color_pixel(Get_pixel_out(image, xc - x, yc + i), color);
	Color_pixel(Get_pixel_out(image, xc + i, yc + x), color);
	Color_pixel(Get_pixel_out(image, xc + i, yc - x), color);
    }
}
