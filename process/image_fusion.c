#include "image_fusion.h"

void Fusion(GdkPixbuf *img, int alpha){
	int height = gdk_pixbuf_get_height(img);
	int width = gdk_pixbuf_get_width(img);

/*	if(!gdk_pixbuf_get_has_alpha(img) && gdk_pixbuf_get_n_channels (img) != 4){
		err(1,"No alpha channel in image.\n");
*/	

	for(int y = 0; y < height ; y++){
		for(int x = 0; x < width ; x++){
			guchar* cur_pixel= Get_pixel(img,x,y);
			cur_pixel[3] = alpha;

		}
	}
//	}

}
