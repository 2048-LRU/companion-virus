#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

int convertToFahrenheit(int celcius) { return (celcius * 9 / 5) + 32; }

int convertToCelcius(int fahrenheit) { return (fahrenheit - 32) * 5 / 9; }

typedef struct {
    GtkWidget *entry;
    GtkWidget *dropdown;
    GtkWidget *label;
} Widgets;

static void do_conversion(Widgets *widgets) {
    const char *text = gtk_editable_get_text(GTK_EDITABLE(widgets->entry));
    guint active = gtk_drop_down_get_selected(GTK_DROP_DOWN(widgets->dropdown));
    char *endPtr;

    if (strlen(text) == 0) {
        gtk_label_set_text(GTK_LABEL(widgets->label), "Waiting for input...");
        return;
    }

    if (active == 0) {  // Celcius → Fahrenheit
        long value = strtol(text, &endPtr, 10);

        int fahrenheit = convertToFahrenheit(value);
        if (text != endPtr) {
            char result[64];
            snprintf(result, sizeof(result), "%d", fahrenheit);
            gtk_label_set_text(GTK_LABEL(widgets->label), result);
        } else {
            gtk_label_set_text(GTK_LABEL(widgets->label), "Invalid input");
        }
    } else {  // Fahrenheit → Celcius
        long value = strtol(text, &endPtr, 10);

        int celcius = convertToCelcius(value);
        if (text != endPtr) {
            char result[64];
            snprintf(result, sizeof(result), "%d", celcius);
            gtk_label_set_text(GTK_LABEL(widgets->label), result);
        } else {
            gtk_label_set_text(GTK_LABEL(widgets->label), "Invalid input");
        }
    }
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Temperature converter");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, -1);

    Widgets *widgets = g_new0(Widgets, 1);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_halign(vbox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(vbox, GTK_ALIGN_CENTER);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    const char *options[] = {"Celcius → Fahrenheit", "Fahrenheit → Celcius",
                             NULL};
    widgets->dropdown = gtk_drop_down_new_from_strings(options);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(widgets->dropdown), 0);
    widgets->entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->entry), "Enter value...");
    widgets->label = gtk_label_new("Result :");
    gtk_widget_add_css_class(widgets->label, "title-2");

    gtk_box_append(GTK_BOX(vbox), widgets->dropdown);
    gtk_box_append(GTK_BOX(vbox), widgets->entry);
    gtk_box_append(GTK_BOX(vbox), widgets->label);

    g_signal_connect_swapped(widgets->entry, "changed",
                             G_CALLBACK(do_conversion), widgets);
    g_signal_connect_swapped(widgets->dropdown, "notify::selected",
                             G_CALLBACK(do_conversion), widgets);

    g_signal_connect_swapped(window, "destroy", G_CALLBACK(g_free), widgets);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("companion.virus.temp_converter",
                                              G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);

    return status;
}