#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/gui_utils.h"
#include "utils/base_converter.h"

typedef struct {
    GtkWidget *entry;
    GtkWidget *dropdown;
    GtkWidget *spin;
    GtkWidget *label;
} Widgets;

static void do_conversion(Widgets *widgets) {
    const char *text = gtk_editable_get_text(GTK_EDITABLE(widgets->entry));
    guint active = gtk_drop_down_get_selected(GTK_DROP_DOWN(widgets->dropdown));
    int base = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(widgets->spin));

    if (strlen(text) == 0) {
        gtk_label_set_text(GTK_LABEL(widgets->label), "Waiting for input...");
        return;
    }

    if (active == 0) {
        char *res = convert_decimal_to(atoi(text), base);
        gtk_label_set_text(GTK_LABEL(widgets->label), res ? res : "Error");
        free_converted(res);
    } else {
        int number = convert_to_decimal(text, base);
        if (number != -1) {
            char result[64];
            snprintf(result, sizeof(result), "%d", number);
            gtk_label_set_text(GTK_LABEL(widgets->label), result);
        } else {
            gtk_label_set_text(GTK_LABEL(widgets->label), "Invalid input");
        }
    }
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = create_standard_window(app, "Bases Converter", 300, -1);
    GtkWidget *vbox = create_padded_box(GTK_ORIENTATION_VERTICAL, 12, 20);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    Widgets *widgets = g_new0(Widgets, 1);

    const char *options[] = {"Decimal → Base", "Base → Decimal", NULL};
    widgets->dropdown = gtk_drop_down_new_from_strings(options);
    widgets->entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->entry),
                                   "Enter number...");
    widgets->spin = gtk_spin_button_new_with_range(2, 36, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(widgets->spin), 10);
    widgets->label = gtk_label_new("Result:");
    gtk_widget_add_css_class(widgets->label, "title-2");

    gtk_box_append(GTK_BOX(vbox), widgets->dropdown);
    gtk_box_append(GTK_BOX(vbox), widgets->entry);
    gtk_box_append(GTK_BOX(vbox), gtk_label_new("Base:"));
    gtk_box_append(GTK_BOX(vbox), widgets->spin);
    gtk_box_append(GTK_BOX(vbox), widgets->label);

    g_signal_connect_swapped(widgets->entry, "changed",
                             G_CALLBACK(do_conversion), widgets);
    g_signal_connect_swapped(widgets->spin, "value-changed",
                             G_CALLBACK(do_conversion), widgets);
    g_signal_connect_swapped(widgets->dropdown, "notify::selected",
                             G_CALLBACK(do_conversion), widgets);
    g_signal_connect_swapped(window, "destroy", G_CALLBACK(g_free), widgets);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("companion.virus.base_converter",
                                              G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
