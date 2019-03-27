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
#include "process/image_crypto.h"
#include "process/image_histogram.h"

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

int test_histogram(char *path)
{
    printf(" ==== Testing histogram method ====\n");
    GdkPixbuf *image = Load_image(path);
    image = Create_histo_graph(image);

    printf("- Black histogram generated\n");
    printf("- Red histogram generated\n");
    printf("- Green histogram generated\n");
    printf("- Blue histogram generated\n");
    printf("- Main histogram generated\n\n");

    return 0;
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
    printf("- Image decrypted\n");
    return 0;
}

int main(int argc, char *argv[])
{
    printf("\n");

    Check_dir("dst/filtre/");

    if (argc == 1)
    {
        test_scale("src/image/img_3.jpg");
        test_drawcircle("src/image/img_1.jpg", 70);
        test_fillcircle("src/image/img_1.jpg", 70);
        test_grayscale("src/image/img_2.jpg");
        test_contrast("src/image/img_4.jpg");
        test_motionblur("src/image/img_sansblur.jpg");
        test_crypto("src/image/img_3.jpg");
        test_histogram("src/image/img_2.jpg");
        printf(" ==== Results in dst/filtre/ ==== \n");
    }
    else if (argc == 3)
    {
        char *function = argv[2];
        if (!strcmp(function, "scale"))
            test_scale(argv[1]);
        else if (!strcmp(function, "drawc"))
            test_drawcircle(argv[1], 70);
        else if (!strcmp(function, "fillc"))
            test_fillcircle(argv[1], 70);
        else if (!strcmp(function, "gray"))
            test_grayscale(argv[1]);
        else if (!strcmp(function, "contra"))
            test_contrast(argv[1]);
        else if (!strcmp(function, "blur"))
            test_motionblur(argv[1]);
        else if (!strcmp(function, "crypto"))
            test_crypto(argv[1]);
        else
            errx(EXIT_FAILURE, "Invalid function name\n \
		    try scale, drawc, fillc, gray, contra, blur");

        printf(" ==== Results in dst/filtre/ ==== \n");
    }
    else
        errx(EXIT_FAILURE, "Invalid args please use the image path");

    return 0;
}
