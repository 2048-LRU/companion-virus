#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "gui_utils.h"

typedef struct {
    GtkWidget *min_entry;
    GtkWidget *max_entry;
    GtkWidget *label;
} Widgets;

int randint(int min, int max) { return (rand() % (max - min + 1)) + min; }

static void generate_number(GtkButton *button, gpointer data) {
    Widgets *widgets = (Widgets *)data;

    const char *min_text =
        gtk_editable_get_text(GTK_EDITABLE(widgets->min_entry));
    const char *max_text =
        gtk_editable_get_text(GTK_EDITABLE(widgets->max_entry));

    int min = atoi(min_text);
    int max = atoi(max_text);

    if (max <= min) {
        gtk_label_set_text(GTK_LABEL(widgets->label), "Invalid range");
        return;
    }

    int random = randint(min, max);

    char result[64];
    snprintf(result, sizeof(result), "Result: %d", random);
    gtk_label_set_text(GTK_LABEL(widgets->label), result);
}

static void activate(GtkApplication *app, gpointer user_data) {
    srand(time(NULL));

    GtkWidget *window = create_standard_window(app, "Random Number Generator", 300, 200);
    Widgets *widgets = g_new0(Widgets, 1);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_widget_set_margin_top(grid, 20);
    gtk_widget_set_margin_bottom(grid, 20);
    gtk_widget_set_margin_start(grid, 20);
    gtk_widget_set_margin_end(grid, 20);
    gtk_window_set_child(GTK_WINDOW(window), grid);

    widgets->min_entry = gtk_entry_new();
    widgets->max_entry = gtk_entry_new();
    GtkWidget *button = gtk_button_new_with_label("Generate");
    widgets->label = gtk_label_new("Result:");

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Min:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), widgets->min_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Max:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), widgets->max_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), widgets->label, 0, 3, 2, 1);

    g_signal_connect(button, "clicked", G_CALLBACK(generate_number), widgets);
    g_signal_connect_swapped(window, "destroy", G_CALLBACK(g_free), widgets);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("companion.virus.num_generator", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}