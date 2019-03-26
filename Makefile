## Makefile

CC=gcc -rdynamic -MMD -D_XOPEN_SOURCE=500

CPPFLAGS= `pkg-config --cflags gtk+-3.0`
CFLAGS= -Wall -Wextra -std=c99 -O3 -g
LDFLAGS=
LDLIBS= `pkg-config --libs gtk+-3.0` -lm

## Cleaning part
SUBDIR_ROOTS := io process filters img_out
DIRS := . $(shell find $(SUBDIR_ROOTS) -type d)
GARBAGE_PATTERNS := *.o *.png *.d *.jpg main
GARBAGE := $(foreach DIR,$(DIRS),$(addprefix $(DIR)/,$(GARBAGE_PATTERNS)))

## Include all C
SRC= main.c io/io.c process/image_process.c process/image_convolution.c \
process/image_circle.c process/image_scale.c process/image_grayscale.c \
filters/contrast.c process/image_binarize.c process/image_crypto.c

OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}


all: main

main: ${OBJ}

clean: 
	rm -rf $(GARBAGE)

-include ${DEP}

# END
