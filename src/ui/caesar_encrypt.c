#include "core/caesar_encrypt.h"

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui/caesar_encrypt_ui.h"
#include "ui/gui_utils.h"

typedef struct {
    GtkWidget *window;
    GtkWidget *spin;
    GtkWidget *label;
    char *filename;
} CypherWidgets;

static void on_file_selected(const char *path, gpointer data) {
    CypherWidgets *cw = data;
    g_free(cw->filename);
    cw->filename = g_strdup(path);
    gtk_label_set_text(GTK_LABEL(cw->label), cw->filename);
    g_print("Selected: %s\n", cw->filename);
}

static void on_open_clicked(GtkButton *button, gpointer data) {
    CypherWidgets *cw = data;
    open_file_dialog(GTK_WINDOW(cw->window), "Open File",
                     GTK_FILE_CHOOSER_ACTION_OPEN, on_file_selected, cw);
}

static void on_cypher_clicked(GtkButton *button, gpointer data) {
    CypherWidgets *cw = data;
    if (cw->filename) {
        int shift = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(cw->spin));
        caesar_cipher_file(cw->filename, shift);
    }
}

static void activate(GtkApplication *app, gpointer user_data) {
    CypherWidgets *cw = g_new0(CypherWidgets, 1);

    cw->window = create_standard_window(app, "Caesar Cypher", 300, 100);
    GtkWidget *box = create_padded_box(GTK_ORIENTATION_HORIZONTAL, 12, 20);
    gtk_window_set_child(GTK_WINDOW(cw->window), box);

    GtkWidget *fbutton = gtk_button_new_with_label("Files...");
    cw->label = gtk_label_new("No file selected");
    GtkWidget *cypher = gtk_button_new_with_label("Cypher file");
    cw->spin = gtk_spin_button_new_with_range(1, 26, 1);

    g_signal_connect(fbutton, "clicked", G_CALLBACK(on_open_clicked), cw);
    g_signal_connect(cypher, "clicked", G_CALLBACK(on_cypher_clicked), cw);

    gtk_box_append(GTK_BOX(box), fbutton);
    gtk_box_append(GTK_BOX(box), cw->label);
    gtk_box_append(GTK_BOX(box), gtk_label_new("shift : "));
    gtk_box_append(GTK_BOX(box), cw->spin);
    gtk_box_append(GTK_BOX(box), cypher);

    gtk_window_present(GTK_WINDOW(cw->window));
}

void caesar_encrypt_ui_activate(GtkApplication *app, gpointer user_data) {
    activate(app, user_data);
}

int main(int argc, char **argv) {
    GtkApplication *app =
        gtk_application_new("companion.virus.caesar", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(caesar_encrypt_ui_activate),
                     NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
