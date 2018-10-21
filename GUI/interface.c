#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "interface.h"
#include "../imageTreatment/image.h"
#include "../imageTreatment/matrix.h"
#include "../imageTreatment/queue.h"
#include "../imageTreatment/segmentation.h"
#include "../neuralNetwork/neuralNet.h"

GtkWidget *window;
GtkImage *imageIni;
GtkImage *imageDone;
GtkWidget *loadButton;
GtkWidget *binButton;
GtkWidget *segButton;


void Display(gchar *path)
{
    //GtkWidget *image = gtk_image_new_from_file(path);
    printf("file=%s\n", path);
    gtk_image_set_from_file(imageIni, path);
}

void Load(GtkWidget *file_chooser)
{
    gchar *path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser));
    printf("%s\n", path);
    Display(path);
}

void Quit()
{
    gtk_widget_destroy(GTK_WIDGET(window));
    gtk_widget_show_all (window);
}

/**************/
/**** MAIN ****/
/**************/

int main(int argc, char *argv[])
{
    GtkBuilder *builder;

    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "interface.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "main"));

    imageIni = GTK_IMAGE(gtk_builder_get_object(builder, "Ini"));
    imageDone = GTK_IMAGE(gtk_builder_get_object(builder, "Final"));
    loadButton = GTK_WIDGET(gtk_builder_get_object(builder, "Load"));
    binButton = GTK_WIDGET(gtk_builder_get_object(builder, "Binarize"));
    segButton = GTK_WIDGET(gtk_builder_get_object(builder, "Segmentation"));

    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);
    gtk_widget_show_all (window);
    gtk_main();

    return 0;
}
