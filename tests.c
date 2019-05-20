#include <gtk-3.0/gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io/io.h"

#include "process/image_binarize.h"
#include "process/image_circle.h"
#include "process/image_crypto.h"
#include "process/image_fusion.h"
#include "process/image_grayscale.h"
#include "process/image_histogram.h"
#include "process/image_process.h"
#include "process/image_reverse.h"
#include "process/image_rotate.h"
#include "process/image_scale.h"
#include "process/image_split.h"

#include "filters/contrast.h"

int test_motionblur(char *path)
{
  struct convolution_matrix *convo = InitMotionBlur(5, 3);
  GdkPixbuf *image = Load_image(path);

  printf(" === Testing convolution function ====\n");
  GdkPixbuf *new_image = Apply_matrix(image, convo);
  Save_pixbuf("dst/filtre/img_convo.png", "png", new_image);
  printf("- Convolution filter applied\n\n");

  printf(" === Testing motion_blur function ====\n");
  GdkPixbuf *blur_hor = Motion_blur_hor(image, 25);
  Save_pixbuf("dst/filtre/img_blurhor.png", "png", blur_hor);
  printf("- Motion blur applied\n\n");

  printf(" === Testing motion_blur function v2 ====\n");
  GdkPixbuf *blur_hor2 = Motion_blur_hor(image, 100);
  Save_pixbuf("dst/filtre/img_blurhor2.png", "png", blur_hor2);
  printf("- Motion blur 2 applied\n\n");

  return 0;
}

int test_contrast(char *path)
{
  int offset = 30;
  GdkPixbuf *image_bright = Load_image(path);
  GdkPixbuf *image_contr = Load_image(path);

  Brightness(image_bright, offset);
  printf(" === Testing brightness function ====\n");
  Save_pixbuf("dst/filtre/img_bright.png", "png", image_bright);
  printf("- Image brightened\n\n");

  Contrast(image_contr, offset);
  printf(" === Testing contrast function ====\n");
  Save_pixbuf("dst/filtre/img_contrast.png", "png", image_contr);
  printf("- Contrast improved\n\n");

  return 0;
}

int test_scale(char *path)
{
  printf(" ==== Testing image_scale method ====\n");
  GdkPixbuf *image = Load_image(path);

  GdkPixbuf *scale_1;
  scale_1 = Scale_neighbors(image, 300, 300);
  Save_pixbuf("dst/filtre/img_rescaled.png", "png", scale_1);
  printf("- Scale_neighbors x300 \n\n");

  return 0;
}

int test_reverse(char *path)
{
  printf(" ==== Testing image_reverse methods ====\n");
  GdkPixbuf *image = Load_image(path);
  GdkPixbuf *image2 = Load_image(path);

  ReverseVert(image);
  ReverseHor(image2);

  Save_pixbuf("dst/filtre/img_reverseVert.png", "png", image);
  Save_pixbuf("dst/filtre/img_reverseHor.png", "png", image2);

  printf("- Image reversed vertically\n");
  printf("- Image reversed horizontally\n\n");

  return 0;
}

int test_rotate(char *path)
{
  printf(" ==== Testing image_rotate method ====\n");
  GdkPixbuf *image = Load_image(path);

  GdkPixbuf *rot1 = Rotate(image, 1);
  Save_pixbuf("dst/filtre/img_rot1x.png", "png", rot1);

  GdkPixbuf *rot2 = Rotate(image, 2);
  Save_pixbuf("dst/filtre/img_rot2x.png", "png", rot2);

  GdkPixbuf *rot3 = Rotate(image, 3);
  Save_pixbuf("dst/filtre/img_rot3x.png", "png", rot3);

  printf("- Image rotated\n\n");

  return 0;
}

int test_drawcircle(char *path, int r)
{
  struct color *Couleur = malloc(sizeof(struct color));
  Couleur->red = 255;
  Couleur->green = 255;
  Couleur->blue = 255;
  Couleur->opacity = 255;
  printf(" ==== Testing Draw_circle method ====\n");
  GdkPixbuf *image = Load_image(path);
  testCircleDraw(image, Couleur, 150, 150, r, 0);
  Save_pixbuf("dst/filtre/img_drawcircle.png", "png", image);
  printf("- Circle drawn (150, 150)\n\n");

  return 0;
}

int test_fillcircle(char *path, int r)
{
  struct color *Couleur = malloc(sizeof(struct color));
  Couleur->red = 255;
  Couleur->green = 255;
  Couleur->blue = 255;
  Couleur->opacity = 255;

  printf(" ==== Testing fill_circle method ====\n");
  GdkPixbuf *image = Load_image(path);
  testCircleDraw(image, Couleur, 150, 150, r, 1);
  Save_pixbuf("dst/filtre/img_fillcircle.png", "png", image);
  printf("- Circle filled (150, 150)\n\n");

  return 0;
}

int test_fillcolor(char *path)
{
  printf(" ==== Testing fillcolor method ====\n");
  GdkPixbuf *image = Load_image(path);
  struct box *toFill = malloc(sizeof(struct box));
  toFill->x1 = 100;
  toFill->y1 = 100;
  toFill->x2 = 500;
  toFill->y2 = 500;
  struct color *col = malloc(sizeof(struct color));
  col->red = 255;
  col->green = 0;
  col->blue = 0;
  col->opacity = 255;
  Fill_color2(image, col, toFill);
  Save_pixbuf("dst/filtre/img_fillcolor.png", "png", image);
  printf("- Area filled with red\n\n");

  return 0;
}

int test_grayscale(char *path)
{
  printf(" ==== Testing grayscale method ====\n");
  GdkPixbuf *image = Load_image(path);
  GrayScale(image);
  Save_pixbuf("dst/filtre/img_grayscale.png", "png", image);
  printf("- Image converted in gray scale\n\n");

  printf(" ==== Testing binarize method ====\n");
  binarize(image, 127);
  Save_pixbuf("dst/filtre/img_binarized.png", "png", image);
  printf("- Image binarized\n\n");

  return 0;
}

int test_fusion(char *path)
{
<<<<<<< HEAD
    printf(" ==== Testing fusion method ====\n");
    GdkPixbuf *image = Load_image(path);
    Fusion(image, 255);
    Save_pixbuf("dst/filtre/img_fusion.png", "png", image);
    printf("- Alpha applied to the image\n\n");
=======
  printf(" ==== Testing fusion method ====\n");
  GdkPixbuf *image = Load_image(path);
  Fusion(image, 125);
  Save_pixbuf("dst/filtre/img_fusion.png", "png", image);
  printf("- Alpha applied to the image\n\n");
>>>>>>> a94f30f2ec61a24af30e5568b83219a3361c015f

  return 0;
}

int test_histogram(char *path)
{
  printf(" ==== Testing histogram method ====\n");
  GdkPixbuf *image = Load_image(path);

  printf("- Black histogram generated\n");
  printf("- Red histogram generated\n");
  printf("- Green histogram generated\n");
  printf("- Blue histogram generated\n");
  printf("- Main histogram generated\n\n");
  return 0;
}

int test_histo_equalisation(char *path)
{
  printf(" ==== Testing histogram equalisation ====\n");
  GdkPixbuf *image = Load_image(path);
  HistogramEqualisation(image);
  Save_pixbuf("dst/filtre/img_histo_equalisation.png", "png", image);
}
int test_histo_normalize(char *path)
{
  printf(" ==== Testing histogram normalize ====\n");
  GdkPixbuf *image = Load_image(path);
  HistogramNormalize(image);
  Save_pixbuf("dst/filtre/img_histo_normalize.png", "png", image);
}

int test_crypto(char *path)
{
  printf(" ==== Testing crypto method ====\n");
  GdkPixbuf *image = Load_image(path);
  GdkPixbuf *code;
  code = Crypto(image);

  Save_pixbuf("dst/filtre/img_encrypted.png", "png", image);
  Save_pixbuf("dst/filtre/code.png", "png", code);

  Uncrypt(image, code);
  Save_pixbuf("dst/filtre/img_decrypted.png", "png", image);

  printf("- Code generated\n");
  printf("- Image crypted \n");
  printf("- Image decrypted\n\n");

  return 0;
}

int test_split(char *path)
{
  printf("==== Testing colors split method ====\n");
  GdkPixbuf *image = Load_image(path);
  GdkPixbuf **result = Split(image);

  Save_pixbuf("dst/filtre/img_splitRed.png", "png", result[0]);
  Save_pixbuf("dst/filtre/img_splitGreen.png", "png", result[1]);
  Save_pixbuf("dst/filtre/img_splitBlue.png", "png", result[2]);

  printf("- Image splitted in 3\n\n");
  return 0;
}

int consoleTests()
{
  printf("\n");

  Check_dir("dst/filtre/");
  Check_dir("dst/histogram/");

  test_histo_equalisation("src/image/img_2.jpg");
  test_histo_normalize("src/image/img_5.jpg");
  test_scale("src/image/img_3.jpg");
  test_reverse("src/image/img_2.jpg");
  test_rotate("src/image/img_2.jpg");
  test_drawcircle("src/image/img_1.jpg", 70);
  test_fillcircle("src/image/img_1.jpg", 70);
  test_fillcolor("src/image/img_3.jpg");
  test_grayscale("src/image/img_2.jpg");
  test_contrast("src/image/img_4.jpg");
  test_motionblur("src/image/img_sansblur.jpg");
  test_crypto("src/image/img_3.jpg");
  test_split("src/image/img_1.jpg");
  test_histogram("src/image/img_2.jpg");
  test_fusion("src/image/img_11.png");
  printf(" ==== Results in dst/filtre/ ==== \n");

  return 0;
}
