#include "image_selection.h"
#include "math.h"
#include "image_process.h"

struct selection *Selection_init(GdkPixbuf *image)
{
  struct selection *sel = calloc(1, sizeof(struct selection));
  sel->height = gdk_pixbuf_get_height(image);
  sel->width = gdk_pixbuf_get_width(image);
  sel->matrix = calloc(1, sel->height * sel->width * sizeof(char));
  sel->image = image;

  return sel;
}

void Selection_free(struct selection *selection)
{
  free(selection->matrix);
  free(selection);
}

void Selection_box(struct selection *sel, struct box *box, char bool)
{
  // FIXME add error management
  for (int x = box->x1; x <= box->x2; ++x)
    for (int y = box->y1; y <= box->y2; ++y)
      sel->matrix[sel->width * y + x] = bool;
}

void Selection_circle(struct selection *sel, int x_center, int y_center, int r, char bool)
{
  if (r < 0)
    return;
  int x = 0;
  int y = r;
  int d = r - 1;
  while (x <= y)
  {
    sel->matrix[x_center + x + (y_center + y) * sel->width] = bool;
    sel->matrix[x_center + y + (y_center + x) * sel->width] = bool;
    sel->matrix[x_center - x + (y_center + y) * sel->width] = bool;
    sel->matrix[x_center - y + (y_center + x) * sel->width] = bool;
    sel->matrix[x_center + x + (y_center - y) * sel->width] = bool;
    sel->matrix[x_center + y + (y_center - x) * sel->width] = bool;
    sel->matrix[x_center - x + (y_center - y) * sel->width] = bool;
    sel->matrix[x_center - y + (y_center - x) * sel->width] = bool;
    if (d >= 2 * x)
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
      d = d + 2 * (y - x - 1);
      --y;
      ++x;
    }
  }
}

void Selection_color(struct selection *sel, struct color *color, float dst, char bool)
{
  for (int x = 0; x < gdk_pixbuf_get_width(sel->image); ++x)
  {
    for (int y = 0; y < gdk_pixbuf_get_height(sel->image); ++y)
    {
      guchar *current = Get_pixel(sel->image, x, y);
      float delta = sqrtf(
          (float)(current[0] * current[0]) + (current[1] * current[1]) + (current[2] * current[2]));
      if (delta < dst)
        sel->matrix[y * sel->width + x] = bool;
    }
  }
}