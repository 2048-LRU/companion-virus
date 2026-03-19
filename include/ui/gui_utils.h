#pragma once

#include <gtk/gtk.h>

typedef void (*FileSelectedCallback)(const char *path, gpointer user_data);

/* Open file dialog with callback */
void open_file_dialog(GtkWindow *parent, 
                      const char *title, 
                      GtkFileChooserAction action, 
                      FileSelectedCallback callback, 
                      gpointer user_data);

GtkWidget* create_standard_window(GtkApplication *app, const char *title, int width, int height);

GtkWidget* create_padded_box(GtkOrientation orientation, int spacing, int margin);
