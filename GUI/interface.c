#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>

#include "interface.h"
#include "../imageTreatment/image.h"
#include "../imageTreatment/matrix.h"
#include "../imageTreatment/queue.h"
#include "../imageTreatment/segmentation.h"
#include "../neuralNetwork/neuralNet.h"
#include "../neuralNetwork/fromqueue.h"

GtkWidget *window;
GtkImage *imageIni;
GtkWidget *loadButton;
GtkWidget *binButton;
GtkWidget *segButton;
GtkTextView *textBox;
GtkWidget *about;
char *s;

int h, w;

Pixel **pixels; // To receive RGB value of the pixels of the image
int   **matrix; // Receives 0 and 1 considering the color of pixel

void Display(gchar *path)
{
    //GtkWidget *image = gtk_image_new_from_file(path);
    printf("file=%s\n", path);
    gtk_image_set_from_file(imageIni, path);
}

void Load(GtkWidget *file_chooser)
{

    // Queue for segmentation
    gchar *path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser));

    printf("path: %s\n", path);
    Display(path);

    SDL_Surface *image = LoadImage(path);

    /*** INIT ***/
    h = image->h;
    w = image->w;

    matrix = InitIntMatrix(h, w);
    pixels = InitPixelMatrix(h, w);

    // Fill the martix
    SurfaceToMatrix(pixels, image, h, w);

    /*** BINARIZATION ***/
    //Greyscale
    GreyScale(pixels, h, w);
}

void PrintText()
{
    // printf("%s\n", s);
    // printf("%lu\n", strlen(s));
    GtkTextBuffer *buffer = gtk_text_buffer_new(NULL);
    gtk_text_buffer_set_text(buffer, s, -1);
    gtk_text_view_set_buffer(textBox, buffer);
    free(s);
}

void Quit()
{
    gtk_main_quit();
    gtk_widget_destroy(GTK_WIDGET(window));
}

void Noise()
{
    Median_Filter(pixels, h , w);
    BinarizeMatrix(pixels, matrix, h, w);

    SDL_SaveBMP(MatrixToSurface(pixels, h, w), "noise.bmp");
    Display("noise.bmp");
}

void Bin()
{
    // Otsu method on matrix
    int threshold = Otsu(pixels, h, w);
    OtsuBinarization(pixels, h, w, threshold);

    BinarizeMatrix(pixels, matrix, h, w);
    BinToPixels(matrix, pixels, h, w);
    SDL_SaveBMP(MatrixToSurface(pixels, h, w), "bin.bmp");

    Display("bin.bmp");
}

void Seg()
{
    Queue *queue;

    queue = Segmentation(matrix, h, w);
    BinToPixels(matrix, pixels, h, w);
    //SDL_SaveBMP(MatrixToSurface(pixels, h, w), "seg.bmp");
    
    s = extractstring("../test.nn", queue);
    
    //Display("seg.bmp");
    PrintText();
}

void About()
{
    gtk_widget_show_all (about);
}

int main(int argc, char *argv[])
{
    GtkBuilder *builder;

    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "interface.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "main"));
    gtk_builder_connect_signals(builder, NULL);

    imageIni    = GTK_IMAGE(gtk_builder_get_object(builder, "Ini"));
    loadButton  = GTK_WIDGET(gtk_builder_get_object(builder, "Load"));
    binButton   = GTK_WIDGET(gtk_builder_get_object(builder, "Binarize"));
    segButton   = GTK_WIDGET(gtk_builder_get_object(builder, "Segmentation"));
    textBox     = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "TextBox"));
    about       = GTK_WIDGET(gtk_builder_get_object(builder, "About"));
    gtk_window_set_transient_for(GTK_WINDOW(about), GTK_WINDOW(window));

    g_object_unref(builder);
    gtk_widget_show_all (window);
    gtk_main();


    /*** FREE ALLOCATED MEMORY ***/
    for (int j = 0; j < h; j++)
    {
        free(pixels[j]);
        free(matrix[j]);
    }

    free(pixels);
    free(matrix);

    return 0;
}
