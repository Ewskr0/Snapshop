#include <stdlib.h>
#include <stdio.h>
#include <gtk-3.0/gtk/gtk.h>
#include <string.h>

#include "io/io.h"

#include "process/image_process.h"
#include "process/image_circle.h"
#include "process/image_scale.h"
#include "process/image_grayscale.h"
#include "process/image_binarize.h"
#include "filters/contrast.h"

int test_motionblur(char *path){                                                
  struct convolution_matrix *convo = InitMotionBlur(5,5);                     
  GdkPixbuf *image = Load_image(path);                                        
  GdkPixbuf *new_image = Apply_matrix(image,convo);                           
  Save_pixbuf("img_out/img_convo.png", "png", new_image);                     
  GdkPixbuf *blur_hor = Motion_blur_hor(image,10);                            
  Save_pixbuf("img_out/img_blurhor.png","png",blur_hor);                      

  return 0;                                                                   
}                                                                               

int test_contrast(char *path){                                                  
  int offset = 30;                                                            
  GdkPixbuf *image_bright = Load_image(path);                                 
  GdkPixbuf *image_contr = Load_image(path);                                  

  Brightness(image_bright,offset);                                            
  Save_pixbuf("img_out/img_bright.png","png",image_bright);                   

  Contrast(image_contr,offset);                                               
  Save_pixbuf("img_out/img_contrast.png","png",image_contr);                  

  return 0;
}

int test_scale(char *path)
{
  printf(" ==== Testing image_scale method ====\n");
  GdkPixbuf *image = Load_image(path);

  GdkPixbuf *scale_1;
  scale_1 = Scale_neighbors(image, 300, 300);
  Save_pixbuf("img_out/img_rescaled.png", "png", scale_1);
  printf("- Scale_neighbors x300 \n");

  return 0;
}

int test_drawcircle(char *path, int r)
{
  struct color *Couleur = malloc(sizeof(struct color));
  Couleur->red = 255; Couleur->green = 255; Couleur->blue = 255;
  Couleur->opacity = 255;
  printf(" === Testing Draw_circle method ====\n");
  GdkPixbuf *image = Load_image(path);
  testCircleDraw(image, Couleur, 150, 150, r, 0);
  Save_pixbuf("img_out/img_drawcircle.png", "png", image);
  printf("- Circle drawn (150, 150)\n");

  return 0;    
}

int test_fillcircle(char *path, int r)
{
  struct color *Couleur = malloc(sizeof(struct color));
  Couleur->red = 255; Couleur->green = 255; Couleur->blue = 255; Couleur->opacity =
    255;
  printf(" === Testing fill_circle method ====\n");
  GdkPixbuf *image = Load_image(path);
  testCircleDraw(image, Couleur, 150, 150, r, 1);
  Save_pixbuf("img_out/img_fillcircle.png", "png", image);
  printf("- Circle filled (150, 150)\n");

  return 0; 
}

int test_grayscale(char *path)
{  
  printf(" ==== Testing image_grayscale method ====\n"); 
  GdkPixbuf *image = Load_image(path);
  GrayScale(image);
  Save_pixbuf("img_out/img_grayscale.png", "png", image);

  printf(" ==== Testing image_binarize method ====\n");
  binarize(image, 127);
  Save_pixbuf("img_out/img_binarized.png", "png", image);
  printf("- Image converted in gray and binarized\n");

  return 0;
}

int main(int argc, char *argv[])                                        
{
  if(argc == 2)
  {
    test_scale(argv[1]);
    test_drawcircle(argv[1], 70);
    test_fillcircle(argv[1],70); 
    test_grayscale(argv[1]); 
    test_contrast(argv[1]);  
    test_motionblur(argv[1]); 

  }
  else if (argc == 3)
  {                                                                 
    char *function = argv[2];
    if(!strcmp(function, "scale"))
      test_scale(argv[1]);
    else if(!strcmp(function,"drawc"))
      test_drawcircle(argv[1],70);
    else if(!strcmp(function,"fillc"))                                                   
      test_fillcircle(argv[1],70);                                                
    else if(!strcmp(function,"gray"))                                                   
      test_grayscale(argv[1]);                                                    
    else if(!strcmp(function ,"contra"))                                                 
      test_contrast(argv[1]);                                                     
    else if(!strcmp(function,"bright"))
      test_motionblur(argv[1]);                                                   
    else errx(EXIT_FAILURE, "Invalid function name\n \
	try scale, drawc, fillc, gray, contra, bright");
    printf(" ==== Results in img_out/ ==== \n");                                  
  }
  else errx
    (EXIT_FAILURE, "Invalid args please use the image path");

  return 0;
}
