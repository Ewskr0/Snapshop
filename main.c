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
		printf("9\n");
	}

	printf("%i\n", top);

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

//////SETTINGS DRAW////////

void settings_draw()
{
	selected_event = 1;
	GtkWidget *drawwindow;
	drawwindow = GTK_WIDGET(gtk_builder_get_object(builder, "draw_window"));
	gtk_widget_show(drawwindow);

	// Récupération de la fenêtre root

	//free(drawwindow);
}

gint x_center, y_center;
int select = 0;

void select_center()
{
	select = 1;
}

void point_center()
{
	GdkWindow *root = NULL;

	// Récupération de la fenêtre root
	root = gdk_get_default_root_window();
	if (select)
	{
		if (root == NULL)
			g_printerr("root = NULL !!!");

		// Récupération du pointeur de la souris
		GdkDevice *pointer = gdk_device_manager_get_client_pointer(gdk_display_get_device_manager(gdk_window_get_display(root)));

		// Récupération des coordonnées de la souris
		gdk_window_get_device_position(root, pointer, &x_center, &y_center, NULL);

		// Mise à jour des GtkLabel pour afficher les coordonnées.
		gchar *text = g_strdup_printf("%d , %d", x_center, y_center);
		gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder, "label_coord")), text);
		select = 0;
		g_free(text);
	}
}

void fermer_draw()
{
	GtkWidget *drawwindow;
	drawwindow = GTK_WIDGET(gtk_builder_get_object(builder, "draw_window"));
	gtk_widget_hide(drawwindow);
	//free(drawwindow);
}

void valider_draw()
{
	GtkToggleButton *fill = (gtk_builder_get_object(builder, "fill"));
	//GtkImage* image_surface2 = GTK_IMAGE(gtk_builder_get_object(builder, "image_display"));
	//GdkPixbuf* image = gtk_image_get_pixbuf(image_surface2);
	GtkEntry *radius_entry = GTK_ENTRY(gtk_builder_get_object(builder, "entry_radius"));
	int radius = atoi((gtk_entry_get_text(radius_entry)));
	struct color color = {0, 0, 0, 0};
	//struct box box = {x_center-radius,y_center}
	/*if(gtk_toggle_button_is_active(rectangle)){
		Fill_color2(image,color,box);		
	}
	else{*/
	testCircleDraw(image_surface, &color, x_center, y_center, radius, gtk_toggle_button_get_active(fill));

	update_image();
	fermer_draw();
}

int select_rect = 0;
struct box box = {0, 0, 0, 0};

void fermer_rect()
{
	GtkWidget *drawwindow;
	drawwindow = GTK_WIDGET(gtk_builder_get_object(builder, "draw_rect"));
	select_rect = 0;
	gtk_widget_hide(drawwindow);
}

void select_rectangle()
{
	selected_event = 1;
	select_rect = 1;
}

void settings_rect()
{
	GtkWidget *drawwindow;
	drawwindow = GTK_WIDGET(gtk_builder_get_object(builder, "5"));
	gtk_widget_show(drawwindow);
}

void point_rect()
{
	GdkWindow *root = NULL;
	gint x, y;
	root = gdk_get_default_root_window();
	if (select_rect > 0)
	{
		printf("%i\n", select_rect);
		if (root == NULL)
			g_printerr("root = NULL !!!");
		gchar *rect;
		// Récupération du pointeur de la souris

		GdkDevice *pointer = gdk_device_manager_get_client_pointer(gdk_display_get_device_manager(gdk_window_get_display(root)));

		// Récupération des coordonnées de la souris
		gdk_window_get_device_position(root, pointer, &x, &y, NULL);
		if (select_rect == 2)
		{
			printf("%i\n", 2);
			box.x2 = x;
			box.y2 = y;
			rect = "rect_2";
		}
		else
		{
			printf("%i\n", 1);
			box.x1 = x;
			box.y1 = y;
			rect = "rect_1";
		}

		// Mise à jour des GtkLabel pour afficher les coordonnées.
		gchar *text = g_strdup_printf("%d , %d", x, y);
		gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder, rect)), text);
		select_rect += 1;
		g_free(text);
	}
}

void valider_rect()
{
	struct color col = {0, 0, 0, 0};
	Fill_color2(image_surface, &col, &box);
	fermer_rect();
	update_image();
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
	GtkWidget *drawwindow;
	drawwindow = GTK_WIDGET(gtk_builder_get_object(builder, "draw_pencil"));
	gtk_widget_show(drawwindow);
	return;
}

struct color pencil_color = {0, 0, 0, 0};
int pencil_radius = 5;
void validate_pencil_radius()
{
	pencil_radius = atoi(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "pencil_radius"))));
}

void close_pencil_radius()
{
	GtkWidget *drawwindow;
	drawwindow = GTK_WIDGET(gtk_builder_get_object(builder, "draw_pencil"));
	gtk_widget_hide(drawwindow);
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
	x = event->x;
	y = event->y;

	if (selected_event == 1)
		return point_rect();
	if (selected_event == 2)
		return draw_pencil(x, y, 10);
}

void motion_notify_event(GtkWidget *widget, GdkEventMotion *event)
{
	if (selected_event < 0 || image_surface == NULL)
		return;
	int x, y;
	x = event->x;
	y = event->y;
	if (selected_event < 0)
		return;
	if (selected_event == 2)
		return draw_pencil(x, y, 10);
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
