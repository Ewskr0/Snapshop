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

#ifndef SNAPSHOP_TESTS_H
#define SNAPSHOP_TESTS_H
int consoleTests();
#endif