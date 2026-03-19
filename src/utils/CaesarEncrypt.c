#include <ctype.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "gui_utils.h"

typedef struct {
    GtkWidget *window;
    GtkWidget *spin;
    GtkWidget *label;
    char *filename;
} CypherWidgets;

char caesar(int dec, char s) {
    if (!isalpha(s)) return s;
    char start = isupper(s) ? 'A' : 'a';
    return start + ((tolower(s) - start + dec + 26) % 26);
}

void cypher_file(const char *src_name, int dec) {
    FILE *src = fopen(src_name, "r");
    if (!src) return;
    char *dir_path = g_path_get_dirname(src_name);
    const char *base_name = g_path_get_basename(src_name);
    char output_path[512];
    snprintf(output_path, sizeof(output_path), "%s/cypher_%s", dir_path, base_name);
    FILE *dst = fopen(output_path, "w");
    if (!dst) { fclose(src); return; }
    int c;
    while ((c = fgetc(src)) != EOF) fputc(caesar(dec, c), dst);
    fclose(src); fclose(dst);
    g_print("File encrypted -> %s\n", output_path);
}

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
        int dec = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(cw->spin));
        cypher_file(cw->filename, dec);
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

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("companion.virus.caesar", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}