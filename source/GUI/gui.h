#ifndef GUI_H
#define GUI_H
#define UNUSED(x) (void)(x)

#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "../sdl/our_sdl.h"
#include "../segmentation/segmentation.h"
#include "../process/process.h"
#include "../network/network.h"
#include "../network/tools.h"
#include <string.h>
#include <err.h>
#include <stdlib.h>
#include <gtk/gtk.h>

typedef struct
{
	GtkBuilder *builder;
	gpointer user_data;
} SGlobalData;


void save_text(GtkButton *button, GtkTextBuffer *buffer);
void load_image(GtkButton *button, GtkImage *image);
void open_image(GtkButton *button, GtkLabel *text_label);
char * UpdatePath(char *filepath,size_t len,char c);
int TrainNeuralNetwork();
int OCR(GtkTextBuffer *buffer);
void InitGUI(int argc, char *argv[]);

#endif