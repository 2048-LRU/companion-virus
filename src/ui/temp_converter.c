#include "core/temp_converter.h"

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui/gui_utils.h"
#include "ui/temp_converter_ui.h"

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

    float value = strtof(text, &endPtr);
    if (text == endPtr) {
        gtk_label_set_text(GTK_LABEL(widgets->label), "Invalid input");
        return;
    }

    char result[64];
    if (active == 0) {
        float fahrenheit = celsius_to_fahrenheit(value);
        snprintf(result, sizeof(result), "%.2f°F", fahrenheit);
    } else if (active == 1) {
        float celsius = fahrenheit_to_celsius(value);
        snprintf(result, sizeof(result), "%.2f°C", celsius);
    } else if (active == 2) {
        float kelvin = celsius_to_kelvin(value);
        snprintf(result, sizeof(result), "%.2fK", kelvin);
    } else {
        float celsius = kelvin_to_celsius(value);
        snprintf(result, sizeof(result), "%.2f°C", celsius);
    }

    gtk_label_set_text(GTK_LABEL(widgets->label), result);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window =
        create_standard_window(app, "Temperature converter", 300, 150);
    GtkWidget *vbox = create_padded_box(GTK_ORIENTATION_VERTICAL, 10, 0);

    gtk_widget_set_halign(vbox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(vbox, GTK_ALIGN_CENTER);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    Widgets *widgets = g_new0(Widgets, 1);

    const char *options[] = {"Celsius → Fahrenheit", "Fahrenheit → Celsius",
                             "Celsius → Kelvin", "Kelvin → Celsius", NULL};
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

void temp_converter_ui_activate(GtkApplication *app, gpointer user_data) {
    activate(app, user_data);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("companion.virus.temp_converter",
                                              G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(temp_converter_ui_activate),
                     NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
