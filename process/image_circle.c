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
void Draw_circle
    (GdkPixbuf *image, struct color *color, int x_center, int y_center, int r)
{
  if (r < 0)
    return;
  int x = 0;
  int y = r;
  int d = r - 1;
  while(x <= y)
  {
    Color_pixel(Get_pixel_out(image, x_center + x, y_center + y), color);
    Color_pixel(Get_pixel_out(image, x_center + y, y_center + x), color);
    Color_pixel(Get_pixel_out(image, x_center - x, y_center + y), color);
    Color_pixel(Get_pixel_out(image, x_center - y, y_center + x), color);
    Color_pixel(Get_pixel_out(image, x_center + x, y_center - y), color);
    Color_pixel(Get_pixel_out(image, x_center + y, y_center - x), color);
    Color_pixel(Get_pixel_out(image, x_center - x, y_center - y), color);
    Color_pixel(Get_pixel_out(image, x_center - y, y_center - x), color);
    if(d >= 2 * x)
    {
      d = d - 2 * x - 1;
      ++x;
    }
    else if (d < 2 * (r - y))
    {
      d = d + 2 * y - 1;
      --y;
    }
    else
    {
     d = d + 2 *(y-x-1);
      --y;
      ++x;
    }
  }
}

void Fill_circle
    (GdkPixbuf *image, struct color *color, int x_center, int y_center, int r)
{
  for(int i = 0; i <= r; ++i)
    Draw_circle(image, color, x_center, y_center, i);
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
