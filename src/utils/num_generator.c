#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

int randint(int min, int max) { return (rand() % (max - min + 1)) + min; }

typedef struct {
    GtkWidget *min_entry;
    GtkWidget *max_entry;
    GtkWidget *label;
} Widgets;

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

    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *min_label;
    GtkWidget *max_label;
    GtkWidget *button;
    GtkWidget *label;

    Widgets *widgets = g_malloc(sizeof(Widgets));

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Random Number Generator");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_widget_set_margin_top(grid, 20);
    gtk_widget_set_margin_bottom(grid, 20);
    gtk_widget_set_margin_start(grid, 20);
    gtk_widget_set_margin_end(grid, 20);
    gtk_window_set_child(GTK_WINDOW(window), grid);

    min_label = gtk_label_new("Min:");
    max_label = gtk_label_new("Max:");

    widgets->min_entry = gtk_entry_new();
    widgets->max_entry = gtk_entry_new();

    button = gtk_button_new_with_label("Generate");

    label = gtk_label_new("Result:");
    widgets->label = label;

    gtk_grid_attach(GTK_GRID(grid), min_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), widgets->min_entry, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), max_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), widgets->max_entry, 1, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), button, 0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), label, 0, 3, 2, 1);

    g_signal_connect(button, "clicked", G_CALLBACK(generate_number), widgets);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("companion.virus.num_generator",
                                              G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);

    return status;
}