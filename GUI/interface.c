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
GtkWidget *imageIni;
GtkWidget *imageDone;
GtkWidget *loadButton;
GtkWidget *binButton;
GtkWidget *segButton;


void Load(GtkWidget * file_chooser)
{
    char *filename =
            gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser));
    printf("%s\n", filename);
    //LoadImage();
}

/**************/
/**** MAIN ****/
/**************/

int StartGUI(int argc, char *argv[])
{
    GtkBuilder *builder;

    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "main.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    if (window == NULL)
    {
        fprintf(stderr, "Unable to file object with id \"window\" \n");
    }

    imageIni = GTK_WIDGET(gtk_builder_get_object(builder, "Ini"));
    imageDone = GTK_WIDGET(gtk_builder_get_object(builder, "Final"));
    loadButton = GTK_WIDGET(gtk_builder_get_object(builder, "Load"));
    binButton = GTK_WIDGET(gtk_builder_get_object(builder, "Binarize"));
    segButton = GTK_WIDGET(gtk_builder_get_object(builder, "Segmentation"));

    gtk_widget_show_all (window);
    //gtk_widget_show(window);
    gtk_main();
    g_object_unref(builder);
    return 0;
}
