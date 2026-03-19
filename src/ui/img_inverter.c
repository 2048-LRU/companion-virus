#include "core/img_inverter.h"

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui/gui_utils.h"
#include "ui/img_inverter_ui.h"

typedef struct {
    GtkWidget *window;
    GtkWidget *label;
    char *filename;
} InverterWidgets;

static void on_file_selected(const char *path, gpointer data) {
    InverterWidgets *iw = data;
    g_free(iw->filename);
    iw->filename = g_strdup(path);
    gtk_label_set_text(GTK_LABEL(iw->label), iw->filename);
    g_print("Selected: %s\n", iw->filename);
}

static void on_open_clicked(GtkButton *button, gpointer data) {
    InverterWidgets *iw = data;
    open_file_dialog(GTK_WINDOW(iw->window), "Choose an Image",
                     GTK_FILE_CHOOSER_ACTION_OPEN, on_file_selected, iw);
}

static void on_invert_clicked(GtkButton *button, gpointer data) {
    InverterWidgets *iw = data;
    if (iw->filename) {
        char *dir_path = g_path_get_dirname(iw->filename);
        const char *base_name = g_path_get_basename(iw->filename);
        char output_path[512];
        snprintf(output_path, sizeof(output_path), "%s/LRU_inverted_%s",
                 dir_path, base_name);

        invert_image_colors(iw->filename, output_path);
        gtk_label_set_text(GTK_LABEL(iw->label), output_path);
    }
}

static void activate(GtkApplication *app, gpointer user_data) {
    InverterWidgets *iw = g_new0(InverterWidgets, 1);

    iw->window = create_standard_window(app, "Image Color Inverter", 400, 150);
    GtkWidget *vbox = create_padded_box(GTK_ORIENTATION_VERTICAL, 12, 20);
    gtk_window_set_child(GTK_WINDOW(iw->window), vbox);

    GtkWidget *fbutton = gtk_button_new_with_label("Select Image...");
    iw->label = gtk_label_new("No image selected");
    gtk_label_set_wrap(GTK_LABEL(iw->label), TRUE);
    GtkWidget *invert_btn = gtk_button_new_with_label("Invert Colors");

    g_signal_connect(fbutton, "clicked", G_CALLBACK(on_open_clicked), iw);
    g_signal_connect(invert_btn, "clicked", G_CALLBACK(on_invert_clicked), iw);

    gtk_box_append(GTK_BOX(vbox), fbutton);
    gtk_box_append(GTK_BOX(vbox), iw->label);
    gtk_box_append(GTK_BOX(vbox), invert_btn);

    gtk_window_present(GTK_WINDOW(iw->window));
}

void img_inverter_ui_activate(GtkApplication *app, gpointer user_data) {
    activate(app, user_data);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("companion.virus.img_inverter",
                                              G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(img_inverter_ui_activate),
                     NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
