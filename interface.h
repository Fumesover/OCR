#ifndef INTERFACE_H
#define INTERFACE_H

char* OpenFileDialog(GtkWidget* button, gpointer window);
void PrintHello (GtkWidget *widget, gpointer data);
void Activate (GtkApplication *app, gpointer user_data);

#endif
