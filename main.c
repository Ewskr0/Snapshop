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
//#include "process/image_selection.h"
#include "process/image_fusion.h"
#include "tests.h"
#include "filters/contrast.h"

///////**STACK FUNCTION**/////////////////////////
int MAXSIZE = 15;
GdkPixbuf *stack[];
int top = -1;
int selected_event = -1;

int isempty()
{

	if (top == -1)
		return 1;
	else
		return 0;
}

int isfull()
{

	if (top == MAXSIZE)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int peek()
{
	return stack[top];
}

GdkPixbuf *pop()
{
	GdkPixbuf *data;

	if (!isempty())
	{
		data = stack[top];
		top = top - 1;
		return data;
	}
	else
	{
		printf("Could not retrieve data, Stack is empty.\n");
	}
}

int push(GdkPixbuf *data)
{

	if (!isfull())
	{
		top = top + 1;
		stack[top] = data;
	}
	else
	{
		printf("Could not insert data, Stack is full.\n");
	}
}

int pop_head()
{
	if (isempty())
		printf("Cannot pop head because stack is empty.\n");
	else
	{
		GdkPixbuf *tmp;
		for (int i = top; i > 1; i--)
		{
			tmp = stack[i - 1];
			stack[i - 1] = stack[i];
		}
		top--;
	}
}
/////////////////////////////////////////////////////////////////////////

GtkBuilder *builder;
GtkEntry *file;

GdkPixbuf *image_surface;
GdkPixbuf *image_code;

int width = 0;
int height = 0;

int ratio = 100;

gchar *fileload;

void buttonload_clicked()
{

	GtkWidget *popup;
	popup = GTK_WIDGET(gtk_builder_get_object(builder, "filechooser"));

	//root = gdk_get_default_root_window ();

	gtk_widget_show(popup);
}

GdkPixbuf *copy_pixbuf(GdkPixbuf *image)
{
	width = gdk_pixbuf_get_width(image);
	height = gdk_pixbuf_get_height(image);
	GdkPixbuf *copy = gdk_pixbuf_copy(image);
	//GdkPixbuf* copy = gdk_pixbuf_new(GDK_COLORSPACE_RGB, 0, 8, width, height);
	/*for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			put
		}
	}*/
	return copy;
}
////////////////////**FILE**//////////////////////////
//////**NEW FILE**///////
void button_new()
{
	GtkWidget *newfile;
	newfile = GTK_WIDGET(gtk_builder_get_object(builder, "newfile"));
	gtk_widget_show(newfile);
}

void fermer_newfile()
{
	GtkWidget *newfile;
	newfile = GTK_WIDGET(gtk_builder_get_object(builder, "newfile"));
	gtk_widget_hide(newfile);
}

void valider_newfile()
{
	int width, height;
	width = atoi(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_width"))));
	height = atoi(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_height"))));

	if (width > 1280)
	{
		ratio = 100 * (1280.0 / (double)width);
	}
	else if (height > 720)
	{
		ratio = 100 * (720.0 / (double)height);
	}

	width = width * ratio / 100;
	height = height * ratio / 100;

	image_surface = gdk_pixbuf_new(GDK_COLORSPACE_RGB, 0, 8, width, height);

	GtkColorChooser *color_chooser;
	color_chooser = GTK_COLOR_CHOOSER(gtk_builder_get_object(builder, "color"));
	GdkRGBA color_choosed;

	gtk_color_chooser_get_rgba(color_chooser, &color_choosed);

	struct color newColor = {((int)(color_choosed.red * 255)), ((int)(color_choosed.green * 255)), ((int)(color_choosed.blue * 255)), 255};

	struct box newBox = {0, 0, width - 1, height - 1};

	Fill_color2(image_surface, &newColor, &newBox);

	GtkImage *image =
			GTK_IMAGE(gtk_builder_get_object(builder, "image_display"));

	GdkPixbuf *res = copy_pixbuf(image_surface);
	push(res);

	gtk_image_set_from_pixbuf(image, image_surface);

	fermer_newfile();

	GtkWidget *tools;
	tools = GTK_WIDGET(gtk_builder_get_object(builder, "tools"));
	gtk_widget_show(tools);
}
/////*SAVE*/////

int asbeenload = 0;

void button_saveas()
{
	GtkWidget *folder = GTK_WIDGET(gtk_builder_get_object(builder, "folderchooser"));
	gtk_widget_show(folder);
}

void button_save()
{
	if (!asbeenload)
	{
		button_saveas();
	}
	else
	{
		GtkImage *image =
				GTK_IMAGE(gtk_builder_get_object(builder, "image_display"));
		GdkPixbuf *pixtosave = gtk_image_get_pixbuf(image);
		gdk_pixbuf_save(pixtosave, fileload, "jpg", NULL);
	}
}

void close_save()
{
	GtkWidget *folder = GTK_WIDGET(gtk_builder_get_object(builder, "folderchooser"));
	gtk_widget_hide(folder);
}

void valider_save()
{
	GtkWidget *folder = GTK_WIDGET(gtk_builder_get_object(builder, "folderchooser"));
	gchar *name = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_folder")));
	gchar *fileName = gtk_file_chooser_get_filename(folder);
	char buffer[80];
	printf("%s et %s\n", name, fileName);
	sprintf(buffer, "%s/%s", fileName, name);
	printf("%s\n", buffer);
	GtkImage *image =
			GTK_IMAGE(gtk_builder_get_object(builder, "image_display"));
	GdkPixbuf *pixtosave = gtk_image_get_pixbuf(image);
	gdk_pixbuf_save(pixtosave, buffer, "jpg", NULL);
	close_save();

	/*free(folder);
	free(name);
	free(fileName);
	free(buffer);
	free(image);
	free(pixtosave);*/
}

/////**Color Dialog**///////////
void button_color()
{
	GtkWidget *color;	
	color = GTK_WIDGET(gtk_builder_get_object(builder, "color"));
	gtk_widget_show(color);
}

void fermer_color()
{
	GtkWidget *color;
	color = GTK_WIDGET(gtk_builder_get_object(builder, "color"));
	settings_update();
	gtk_widget_hide(color);
}

void valider_color()
{
	GtkColorChooser *color;
	color = GTK_COLOR_CHOOSER(gtk_builder_get_object(builder, "color"));
	GdkRGBA *choosed_color = malloc(sizeof(GdkRGBA));

	gtk_color_chooser_get_rgba(color, choosed_color);

	GtkColorButton *color_button;
	GdkColor *color_transform = malloc(sizeof(GdkColor));
	color_transform->red = (guint16)(65535 * (choosed_color->red));
	color_transform->blue = (guint16)(65535 * (choosed_color->blue));
	color_transform->green = (guint16)(65535 * (choosed_color->green));

	color_button = GTK_COLOR_BUTTON(gtk_builder_get_object(builder, "button_color"));
	gtk_color_button_set_color(color_button, color_transform);
	color_button = GTK_COLOR_BUTTON(gtk_builder_get_object(builder, "button_color2"));
	gtk_color_button_set_color(color_button, color_transform);
	
	fermer_color();
}
//////////////////////////////
//////////////////////////
void tools_show()
{
	GtkWidget *tools;
	tools = GTK_WIDGET(gtk_builder_get_object(builder, "tools"));
	gtk_widget_show(tools);
	//free(tools);
}

void tools_hide()
{
	GtkWidget *tools;
	tools = GTK_WIDGET(gtk_builder_get_object(builder, "tools"));
	gtk_widget_hide(tools);
	//free(tools);
}

int is_jpg(gchar *string)
{
	int i = 0;
	while (string[i] != '\0')
	{
		i++;
		if (string[i] == '.')
		{
			return string[i + 1] == 'j' && string[i + 2] == 'p' && string[i + 3] == 'g';
			//|| string[i+1]=='p' && string[i+2]=='n' && string[i+3]=='g';
		}
	}
	return 0;
}

void fermer_filechooser()
{
	GtkWidget *filechooser;
	filechooser = GTK_WIDGET(gtk_builder_get_object(builder, "filechooser"));
	gtk_widget_hide(filechooser);
	//free(filechooser);
}

void valider_filechooser()
{
	GtkFileChooser *filechooser;
	//	GtkButton *button1 = button;
	//	GtkEntry *status;
	//	status = GTK_ENTRY(gtk_builder_get_object(builder, "status"));
	filechooser =
			GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "filechooser"));
	fileload = gtk_file_chooser_get_filename(filechooser);

	fermer_filechooser();
	image_surface = gdk_pixbuf_new_from_file(fileload, NULL);
	width = gdk_pixbuf_get_width(image_surface);
	height = gdk_pixbuf_get_height(image_surface);

	if (width > 1280)
	{
		ratio = 100 * (1280.0 / (double)width);
	}
	else if (height > 720)
	{
		ratio = 100 * (720.0 / (double)height);
	}

	image_surface = Scale_bilinear(image_surface, ratio, ratio);

	GtkImage *image =
			GTK_IMAGE(gtk_builder_get_object(builder, "image_display"));

	GdkPixbuf *res = copy_pixbuf(image_surface);
	push(res);

	gtk_image_set_from_pixbuf(image, image_surface);
	/*	}
else
	{
		gtk_entry_set_text(status, "File must be *.jpg");
	}*/

	tools_show();
	asbeenload = 1;
	fermer_filechooser();

	//free(filechooser);
	//free(image);
	//free(res);
}

///////////*Tools*/////////////////
int undo_firsttime = 1;

void update_image()
{
	if (top >= MAXSIZE)
	{
		pop_head();
		//printf("9\n");
	}

	//printf("%i\n", top);

	GdkPixbuf *res = copy_pixbuf(image_surface);
	push(res);
	//free(res);

	GtkImage *image =
			GTK_IMAGE(gtk_builder_get_object(builder, "image_display"));

	gtk_image_set_from_pixbuf(image, image_surface);
	undo_firsttime = 1;

	//free(image);
}

void greyscale_button()
{
	GrayScale(image_surface);
	update_image();
}

void negative_button()
{
	Negative(image_surface);
	update_image();
}

void brigthness_button()
{
	Brightness(image_surface, 100);
	update_image();
}

void encrypt_button()
{
	image_code = Crypto(image_surface);
	update_image();
}

void uncrypt_button()
{
	Uncrypt(image_surface, image_code);
	update_image();
}

void reverse_vert_button()
{
	ReverseVert(image_surface);
	update_image();
}

void reverse_hor_button()
{
	ReverseHor(image_surface);
	update_image();
}
int rotation = 1;
void rotation_button()
{
	image_surface = Rotate(image_surface, rotation);
	rotation++;
	update_image();
}

//////  SETTINGS  CIRCLE   ////////

int radius_circle = 10;
int fill_circle = 0;
struct color circle_color = {255, 120, 0, 100};

void button_circle()
{
	selected_event = 1;
}

void settings_circle()
{
	GtkColorChooser *color_chooser;
	color_chooser = GTK_COLOR_CHOOSER(gtk_builder_get_object(builder, "color"));
	GdkRGBA color_choosed;

	gtk_color_chooser_get_rgba(color_chooser, &color_choosed);

	struct color newColor = {((int)(color_choosed.red * 255)), ((int)(color_choosed.green * 255)), ((int)(color_choosed.blue * 255)), 255};
	circle_color = newColor;
	
	radius_circle = atoi(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "entry_radius"))));
	
	GtkToggleButton *fill = (gtk_builder_get_object(builder, "circleFillCheck"));
	fill_circle = gtk_toggle_button_get_active(fill);
	
	// Récupération de la fenêtre root

	//free(drawwindow);
}

void draw_circle(int x, int y)
{
	testCircleDraw(image_surface, &circle_color, x, y, radius_circle, fill_circle);
	update_image();
}

//////////RECTANGLE////////

struct box draw_rect_box = {0, 0, 0, 0};
struct color rect_color = {0, 2550, 0, 100};
int fill_rect = 0;

void button_rect(){
	selected_event = 3;
}

void settings_rect()
{
	GtkColorChooser *color_chooser;
	color_chooser = GTK_COLOR_CHOOSER(gtk_builder_get_object(builder, "color"));
	GdkRGBA color_choosed;

	gtk_color_chooser_get_rgba(color_chooser, &color_choosed);

	struct color newColor = {((int)(color_choosed.red * 255)), ((int)(color_choosed.green * 255)), ((int)(color_choosed.blue * 255)), 255};
	rect_color = newColor;
}

void draw_rect(int x, int y)
{
	draw_rect_box.x1 = x;
	draw_rect_box.y1 = y;
}

void draw_rect2(int x, int y)
{
	
	if (x < draw_rect_box.x1)
	{
		draw_rect_box.x2 = draw_rect_box.x1;
		draw_rect_box.x1 = x;
	}
	else
	{
		draw_rect_box.x2 = x;
	}
	if (y < draw_rect_box.y1)
	{
		draw_rect_box.y2 = draw_rect_box.y1;
		draw_rect_box.y1 = y;
	}
	else
	{
		draw_rect_box.y2 = y;
	}

	Fill_color2(image_surface, &rect_color, &draw_rect_box);
	update_image();
}

void valider_rect()
{
	GtkToggleButton *fill = (gtk_builder_get_object(builder, "rectFillCheck"));
	fill_rect = gtk_toggle_button_get_active(fill);
}

void undo_button()
{
	if (top > -1)
	{
		if (undo_firsttime)
		{
			pop();
		}
		GdkPixbuf *res = pop();
		GtkImage *image =
				GTK_IMAGE(gtk_builder_get_object(builder, "image_display"));

		gtk_image_set_from_pixbuf(image, res);
		undo_firsttime = 0;
	}
}

void pencil_button()
{
	selected_event = 2;
	return;
}

struct color pencil_color = {30, 255, 255, 100};
int pencil_radius = 5;
void settings_pencil()
{
	GtkColorChooser *color_chooser;
	color_chooser = GTK_COLOR_CHOOSER(gtk_builder_get_object(builder, "color"));
	GdkRGBA color_choosed;

	gtk_color_chooser_get_rgba(color_chooser, &color_choosed);

	struct color newColor = {((int)(color_choosed.red * 255)), ((int)(color_choosed.green * 255)), ((int)(color_choosed.blue * 255)), 255};
	pencil_color = newColor;
	pencil_radius = atoi(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "radius"))));
}

void draw_pencil(int x, int y, int r)
{
	Fill_circle(image_surface, &pencil_color, x, y, pencil_radius);
	update_image();
}

void button_press_event(GtkWidget *widget, GdkEventButton *event)
{
	if (selected_event < 0 || image_surface == NULL)
		return;
	int x, y;
	x = (width - 1280) / 2 + event->x;
	y = (height - 720) / 2 + event->y;

	if (selected_event == 1)
		return draw_circle(x, y);
	if (selected_event == 2)
		return draw_pencil(x, y, 10);
	if (selected_event == 3)
		return draw_rect(x, y);
}

void button_release_event(GtkWidget *widget, GdkEventButton *event)
{
	if (selected_event < 0 || image_surface == NULL)
		return;
	int x, y;
	x = (width - 1280) / 2 + event->x;
	y = (height - 720) / 2 + event->y;

	if (selected_event == 3)
		return draw_rect2(x, y);
}

void motion_notify_event(GtkWidget *widget, GdkEventMotion *event)
{
	if (selected_event < 0 || image_surface == NULL)
		return;
	int x, y;
	x = (width - 1280) / 2 + event->x;
	y = (height - 720) / 2 + event->y;

	if (selected_event < 0)
		return;
	if (selected_event == 2)
		return draw_pencil(x, y, 10);
}
void settings_update()
{
	settings_circle();
	settings_pencil();
	settings_rect();
}
int main(int argc,
				 char **argv)
{
	if (argc > 1)
	{
		consoleTests();
		return 1;
	}
	//Interface/////////////////////////////////////////////

	GtkWidget *window;
	GtkWidget *eventbox;

	GError *error = NULL;
	/* Init GTK+ */
	gtk_init(&argc, &argv);
	/* Create new GtkBuilder object */
	builder = gtk_builder_new();

	/* Load UI from file. If error occurs, report it and quit application.
     * Replace "tut.glade" with your saved project. */
	if (!gtk_builder_add_from_file(builder, "interface1.glade", &error))
	{
		g_warning("%s", error->message);
		g_free(error);
		return (1);
	}
	/* Get main window pointer from UI */
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
	eventbox = GTK_WIDGET(gtk_builder_get_object(builder, "eventbox"));

	gtk_widget_set_events(eventbox, GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK | GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK);
	gtk_builder_connect_signals(builder, NULL);
	/* Destroy builder, since we don't need it anymore */
	//g_object_unref( G_OBJECT( builder ) );

	/* Show window. All other widgets are automatically shown by GtkBuilder */
	gtk_widget_show(window);
	/* Start main loop */
	gtk_main();

	//free(image_surface);

	return (0);
}
