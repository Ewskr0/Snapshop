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
#include "tests.h"
#include "filters/contrast.h"

///////////////////////////////////////////////////////

GtkBuilder *builder;
GtkEntry *file;

GdkPixbuf *image_surface;
GdkPixbuf *image_code;

int width = 0;
int height = 0;

int ratio = 100;

GdkPixbuf *unredo[150];
int index = 0;
int max = 0;

int selected_event = -1;

char filesave[80];

void GdkSetCursor(char *name)
{

	GdkScreen *screen = gdk_screen_get_default();
	GdkWindow *win = gdk_screen_get_root_window(screen);

	GdkDisplay *display = gdk_display_get_default();
	GdkCursor *cursor = malloc(sizeof(GdkCursor *));
	if (name == "pencil")
	{
		cursor = gdk_cursor_new(GDK_PENCIL);
	}
	else
	{
		cursor = gdk_cursor_new_from_name(display, name);
	}
	gdk_window_set_cursor(win, cursor);
	while (gtk_events_pending())
		gtk_main_iteration();
}

void buttonload_clicked()
{
	GdkSetCursor("default");
	GtkWidget *popup;
	popup = GTK_WIDGET(gtk_builder_get_object(builder, "filechooser"));
	GtkWidget *window1;
	window1 = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
	gtk_window_set_transient_for(popup, window1);
	gtk_widget_show(popup);
}

GdkPixbuf *copy_pixbuf(GdkPixbuf *image)
{
	width = gdk_pixbuf_get_width(image);
	height = gdk_pixbuf_get_height(image);
	GdkPixbuf *copy = gdk_pixbuf_copy(image);
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
	index = 0;
	max = 0;
	unredo[index] = res;

	gtk_image_set_from_pixbuf(image, image_surface);

	fermer_newfile();

	GtkWidget *tools;
	tools = GTK_WIDGET(gtk_builder_get_object(builder, "tools"));
	gtk_widget_show(tools);
}
/////*SAVE*/////

int asbeensaved = 0;

void button_saveas()
{
	GdkSetCursor("default");
	GtkWidget *folder = GTK_WIDGET(gtk_builder_get_object(builder, "folderchooser"));
	gtk_widget_show(folder);
}

void button_save()
{
	GdkSetCursor("default");
	if (!asbeensaved)
	{
		button_saveas();
	}
	else
	{
		GtkImage *image =
				GTK_IMAGE(gtk_builder_get_object(builder, "image_display"));
		GdkPixbuf *pixtosave = gtk_image_get_pixbuf(image);
		printf("Your image has been saved !\n");
		gdk_pixbuf_save(pixtosave, filesave, "png", NULL);
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
	sprintf(filesave, "%s/%s", fileName, name);
	GtkImage *image =
			GTK_IMAGE(gtk_builder_get_object(builder, "image_display"));
	GdkPixbuf *pixtosave = gtk_image_get_pixbuf(image);
	printf("Your image has been saved !\n");
	gdk_pixbuf_save(pixtosave, filesave, "png", NULL);
	asbeensaved = 1;
	close_save();
}

/////**Color Dialog**///////////
void button_color()
{
	GdkSetCursor("default");
	GtkWidget *color;
	GdkSetCursor("default");
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
	filechooser =
			GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "filechooser"));
	gchar *fileload = gtk_file_chooser_get_filename(filechooser);

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
	index = 0;
	max = 0;
	unredo[index] = res;

	gtk_image_set_from_pixbuf(image, image_surface);

	tools_show();
	fermer_filechooser();
}

///////////*Tools*/////////////////

void update_image()
{
	GdkPixbuf *res = copy_pixbuf(image_surface);
	index++;
	unredo[index] = res;
	max = index;
	GtkImage *image =
			GTK_IMAGE(gtk_builder_get_object(builder, "image_display"));
	gtk_image_set_from_pixbuf(image, image_surface);
}

void greyscale_button()
{
	GrayScale(image_surface);
	GdkSetCursor("default");
	update_image();
}

void negative_button()
{
	Negative(image_surface);
	GdkSetCursor("default");
	update_image();
}

void brigthness_button()
{
	Brightness(image_surface, 100);
	GdkSetCursor("default");
	update_image();
}

void encrypt_button()
{
	GdkSetCursor("default");
	image_code = Crypto(image_surface);
	update_image();
}

void uncrypt_button()
{
	Uncrypt(image_surface, image_code);
	GdkSetCursor("default");
	update_image();
}

void reverse_vert_button()
{
	ReverseVert(image_surface);
	GdkSetCursor("default");
	update_image();
}

void reverse_hor_button()
{
	ReverseHor(image_surface);
	GdkSetCursor("default");
	update_image();
}
void rotation_button()
{
	image_surface = Rotate(image_surface, 1);
	update_image();
}

void blur_button_1()
{
	GdkSetCursor("default");
	int size = atoi(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "blur_size_convo"))));
	int val = atoi(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "blur_val_convo"))));

	struct convolution_matrix *convo = InitMotionBlur(size, val);
	GdkPixbuf *new_image = Apply_matrix(image_surface, convo);
	image_surface = new_image;
	update_image();
}
void blur_button_2()
{
	GdkSetCursor("default");
	int size = atoi(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "blur_size_motion"))));
	GdkPixbuf *new_image = Motion_blur_hor(image_surface, size);
	image_surface = new_image;
	update_image();
}

//////  SETTINGS  CIRCLE   ////////

int radius_circle = 10;
int fill_circle = 0;
struct color circle_color = {255, 120, 0, 100};

void button_circle()
{
	GdkSetCursor("default");
	selected_event = 1;
	settings_circle();
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
}

void histogram_hide_window()
{
	GtkWidget *histo_window = GTK_WIDGET(gtk_builder_get_object(builder, "histogram_window"));
	gtk_widget_hide(histo_window);
}

void histogram_button_1()
{
	GdkPixbuf *img_histo = Create_histo_graph(image_surface);
	GtkWidget *histo_window = GTK_WIDGET(gtk_builder_get_object(builder, "histogram_window"));
	GtkImage *image = GTK_IMAGE(gtk_builder_get_object(builder, "image_histogram"));
	if (img_histo)
		gtk_image_set_from_pixbuf(image, img_histo);
	gtk_widget_show(histo_window);
}
histogram_button_2()
{
	HistogramEqualisation(image_surface);
	update_image();
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

void button_rect()
{
	selected_event = 3;
	GdkSetCursor("crosshair");
	settings_rect();
}

void settings_rect()
{
	GtkColorChooser *color_chooser;
	color_chooser = GTK_COLOR_CHOOSER(gtk_builder_get_object(builder, "color"));
	GdkRGBA color_choosed;

	gtk_color_chooser_get_rgba(color_chooser, &color_choosed);
	GtkToggleButton *fill = (gtk_builder_get_object(builder, "rectFillCheck"));
	fill_rect = gtk_toggle_button_get_active(fill);

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
	if (fill_rect)
		Fill_color2(image_surface, &rect_color, &draw_rect_box);
	else
		Draw_rect(image_surface, &rect_color, &draw_rect_box);
	update_image();
}

void undo_button()
{
	GtkToggleButton *fill = (gtk_builder_get_object(builder, "rectFillCheck"));
	fill_rect = gtk_toggle_button_get_active(fill);

	if (index > 0)
	{
		GtkImage *image =
				GTK_IMAGE(gtk_builder_get_object(builder, "image_display"));
		index -= 1;
		image_surface = copy_pixbuf(unredo[index]);
		gtk_image_set_from_pixbuf(image, image_surface);
	}
}

void redo_button()
{

	if (index < max && max < 15)
	{
		GtkImage *image =
				GTK_IMAGE(gtk_builder_get_object(builder, "image_display"));
		index += 1;
		image_surface = copy_pixbuf(unredo[index]);
		gtk_image_set_from_pixbuf(image, image_surface);
	}
}

void pencil_button()
{
	selected_event = 2;
	GdkSetCursor("pencil");
	settings_pencil();
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

	GtkImage *image = GTK_IMAGE(gtk_builder_get_object(builder, "image_display"));
	gtk_image_set_from_pixbuf(image, image_surface);
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
	if (selected_event == 2)
		update_image();
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
