#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/gui_utils.h"
#include "utils/file_hasher.h"

typedef struct {
    GtkWidget *window;
    GtkWidget *label;
    char *filename;
} HasherWidgets;

static void on_file_selected(const char *path, gpointer data) {
    HasherWidgets *hw = data;
    g_free(hw->filename);
    hw->filename = g_strdup(path);
    gtk_label_set_text(GTK_LABEL(hw->label), hw->filename);
    g_print("Selected: %s\n", hw->filename);
}

static void on_open_clicked(GtkButton *button, gpointer data) {
    HasherWidgets *hw = data;
    open_file_dialog(GTK_WINDOW(hw->window), "Open File",
                     GTK_FILE_CHOOSER_ACTION_OPEN, on_file_selected, hw);
}

static void on_hash_clicked(GtkButton *button, gpointer data) {
    HasherWidgets *hw = data;
    if (hw->filename) {
        char *hash = hash_file_sha256(hw->filename);
        if (hash) {
            gtk_label_set_text(GTK_LABEL(hw->label), hash);
            free_hash(hash);
        } else {
            gtk_label_set_text(GTK_LABEL(hw->label), "Error computing hash");
        }
    }
}

static void activate(GtkApplication *app, gpointer user_data) {
    HasherWidgets *hw = g_new0(HasherWidgets, 1);

    hw->window = create_standard_window(app, "File Hasher (SHA256)", 500, 150);
    GtkWidget *vbox = create_padded_box(GTK_ORIENTATION_VERTICAL, 12, 20);
    gtk_window_set_child(GTK_WINDOW(hw->window), vbox);

    GtkWidget *fbutton = gtk_button_new_with_label("Select File...");
    hw->label = gtk_label_new("No file selected");
    gtk_label_set_wrap(GTK_LABEL(hw->label), TRUE);
    GtkWidget *hash_btn = gtk_button_new_with_label("Compute SHA256");

    g_signal_connect(fbutton, "clicked", G_CALLBACK(on_open_clicked), hw);
    g_signal_connect(hash_btn, "clicked", G_CALLBACK(on_hash_clicked), hw);

    gtk_box_append(GTK_BOX(vbox), fbutton);
    gtk_box_append(GTK_BOX(vbox), hw->label);
    gtk_box_append(GTK_BOX(vbox), hash_btn);

    gtk_window_present(GTK_WINDOW(hw->window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("companion.virus.file_hasher",
                                              G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
