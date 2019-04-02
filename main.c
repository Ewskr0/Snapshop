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
#include "process/image_reverse.h"
#include "process/image_rotate.h"
#include "process/image_split.h"
#include "process/image_fusion.h"

#include "filters/contrast.h"

GtkBuilder *builder;
GtkEntry *file;

GdkPixbuf *image_surface;

int width = 0;
int height = 0;

int ratio = 100;


void buttonload_clicked(GtkButton *button){
    
    GtkWidget *popup; 
    popup = GTK_WIDGET(gtk_builder_get_object(builder,"filechooser"));
    

    gtk_widget_show(popup);

}
//////**NEW FILE**///////
void button_new(GtkButton *button){
	GtkWidget *newfile;
	newfile = GTK_WIDGET(gtk_builder_get_object(builder,"newfile"));
	gtk_widget_show(newfile);
}

void fermer_newfile(){
    GtkWidget* newfile;
    newfile = GTK_WIDGET(gtk_builder_get_object(builder,"newfile"));
    gtk_widget_hide(newfile);
}

void valider_newfile(){
	int width, height;
	width = atoi(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder,"entry_width"))));
	height = atoi(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder,"entry_height"))));	

	if(width>1280){
		ratio = 100*(1280.0/(double)width);
	}	
	else if(height>720){
		ratio = 100*(720.0/(double)height);
	}

	width = width*ratio/100;
	height = height*ratio/100;

	image_surface = gdk_pixbuf_new(GDK_COLORSPACE_RGB,0,8,width,height);

	/*GtkColorChooser *color_chooser;
	color_chooser = GTK_COLOR_CHOOSER(gtk_builder_get_object(builder,"color"));	
	GdkRGBA* color_choosed = malloc(sizeof(GdkRGBA));
	
	gtk_color_chooser_get_rgba(color_chooser,color_choosed);
	
	/*struct color *color = malloc(sizeof(struct color));
	//printf("%f\n",(guchar)(255*(color_choosed->red)));
        color->red = (guchar)(255*(color_choosed->red));
        color->green = (guchar)(255*(color_choosed->green));
        color->blue = (guchar)(255*(color_choosed->blue));
	color->opacity = 1;
	
	
        struct box *box = malloc(sizeof(struct box));
        box->x1 = 0;
        box->x2 = 1280;//gdk_pixbuf_get_width(image_surface);
        box->y1 = 0;
        box->y2 = 720;//gdk_pixbuf_get_height(image_surface);

	Fill_color(image_surface,color,box);*/

	
	GtkImage *image = 
		    GTK_IMAGE(gtk_builder_get_object(builder,"image_display"));
	
	gtk_image_set_from_pixbuf(image,image_surface);
	

	/*free(color);
	free(box);
	free(color_choosed);*/
	fermer_newfile();

	GtkWidget* tools;
	tools = GTK_WIDGET(gtk_builder_get_object(builder,"tools"));
	gtk_widget_show(tools);
}
	/////**Color Dialog**///////////
void button_color(){
	GtkWidget *color;
	color = GTK_WIDGET(gtk_builder_get_object(builder,"color"));
	gtk_widget_show(color);
}

void fermer_color(){
	GtkWidget *color;
	color = GTK_WIDGET(gtk_builder_get_object(builder,"color"));
	gtk_widget_hide(color);
}

void valider_color(){
	GtkColorChooser *color;
	color = GTK_COLOR_CHOOSER(gtk_builder_get_object(builder,"color"));
	GdkRGBA *choosed_color = malloc(sizeof(GdkRGBA));

	gtk_color_chooser_get_rgba(color,choosed_color);
	
	GtkColorButton *color_button;
	GdkColor *color_transform = malloc(sizeof(GdkColor));
	color_transform->red = (guint16)(65535*(choosed_color->red));
	color_transform->blue = (guint16)(65535*(choosed_color->blue));
	color_transform->green = (guint16)(65535*(choosed_color->green));

	color_button = GTK_COLOR_BUTTON(gtk_builder_get_object(builder,"button_color"));
	gtk_color_button_set_color(color_button,color_transform);
	
	free(color_transform);
	free(choosed_color);
	fermer_color();
}
	//////////////////////////////
//////////////////////////

int is_jpg(gchar *string){
    int i = 0;
    while(string[i] != '\0'){
	i++;
	if(string[i] == '.'){
	    return string[i+1]=='j' && string[i+2]=='p' && string[i+3]=='g' 
		|| string[i+1]=='p' && string[i+2]=='n' && string[i+3]=='g';
	}
    }
    return 0;
}

void fermer_filechooser(){
    GtkWidget* filechooser;
    filechooser = GTK_WIDGET(gtk_builder_get_object(builder,"filechooser"));
    gtk_widget_hide(filechooser);

}


void valider_filechooser(GtkButton *button){
    GtkFileChooser *filechooser;
    GtkButton *button1 = button;
    GtkEntry *status;
    status = GTK_ENTRY(gtk_builder_get_object(builder,"status"));
    filechooser =
        GTK_FILE_CHOOSER(gtk_builder_get_object(builder,"filechooser"));
    gchar *fileName = gtk_file_chooser_get_filename(filechooser);

	if(is_jpg(fileName)){
	fermer_filechooser(button1);
	image_surface = gdk_pixbuf_new_from_file(fileName,NULL);
	width = gdk_pixbuf_get_width(image_surface);
	height = gdk_pixbuf_get_height(image_surface);

	if(width>1280){
		ratio = 100*(1280.0/(double)width);
	}	
	else if(height>720){
		ratio = 100*(720.0/(double)height);
	}

	image_surface = Scale_bilinear(image_surface,ratio,ratio);

	GtkImage *image = 
	    GTK_IMAGE(gtk_builder_get_object(builder,"image_display"));
	
	gtk_image_set_from_pixbuf(image,image_surface);
	}
	else{
	    gtk_entry_set_text(status,"File must be *.jpg");
	}

	tools_show();

	fermer_filechooser();
}

void tools_show(){
	GtkWidget* tools;
	tools = GTK_WIDGET(gtk_builder_get_object(builder,"tools"));
	gtk_widget_show(tools);
	
}

void tools_hide(){
	GtkWidget* tools;
	tools = GTK_WIDGET(gtk_builder_get_object(builder,"tools"));
	gtk_widget_hide(tools);

}


///////////*Tools*/////////////////
void update_image(){
	GtkImage *image = 
		    GTK_IMAGE(gtk_builder_get_object(builder,"image_display"));
	
	gtk_image_set_from_pixbuf(image,image_surface);
	
}

void greyscale_button(){
	GrayScale(image_surface);
	update_image();
}


void negative_button(){
	//Negative(image_surface);
	update_image();
}

void brigthness_button(){
	Brightness(image_surface,0);
	update_image();
}

int main( int    argc,
      char **argv )
{

    //Interface/////////////////////////////////////////////

    GtkWidget  *window;
    GError     *error = NULL;
    /* Init GTK+ */
    gtk_init( &argc, &argv );
    /* Create new GtkBuilder object */
    builder = gtk_builder_new();
    /* Load UI from file. If error occurs, report it and quit application.
     * Replace "tut.glade" with your saved project. */
    if( ! gtk_builder_add_from_file( builder, "interface1.glade", &error ) )
    {
        g_warning( "%s", error->message );
        g_free( error );
        return( 1 );
    }
    /* Get main window pointer from UI */
    window = GTK_WIDGET(gtk_builder_get_object( builder, "window1" ));
    
    gtk_builder_connect_signals(builder,NULL);
    /* Destroy builder, since we don't need it anymore */
    //g_object_unref( G_OBJECT( builder ) );
     
    /* Show window. All other widgets are automatically shown by GtkBuilder */
    gtk_widget_show( window );
    /* Start main loop */
    gtk_main();



    return( 0 );
}
