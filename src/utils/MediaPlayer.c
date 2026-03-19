#include <gtk/gtk.h>
#include <gd.h>
#include "gui_utils.h"

typedef struct {
    GtkWidget *window;
    GtkWidget *image_widget;
    GList *image_list;
    GList *current_image;
} AppData;

void display_current(AppData *ad) {
    if (ad->current_image && ad->current_image->data) {
        gtk_image_set_from_file(GTK_IMAGE(ad->image_widget), (char *)ad->current_image->data);
    }
}

static void on_folder_selected(const char *path, gpointer data) {
    AppData *ad = data;

    if (ad->image_list) {
        g_list_free_full(ad->image_list, g_free);
        ad->image_list = NULL;
    }

    GDir *dir = g_dir_open(path, 0, NULL);
    if (dir) {
        const char *name;
        while ((name = g_dir_read_name(dir))) {
            if (g_str_has_suffix(name, ".png") || g_str_has_suffix(name, ".jpg") || g_str_has_suffix(name, ".bmp")) {
                ad->image_list = g_list_append(ad->image_list, g_build_filename(path, name, NULL));
            }
        }
        g_dir_close(dir);
    }
    
    ad->current_image = ad->image_list;
    display_current(ad);
}

static void on_open_folder_clicked(GtkButton *btn, gpointer data) {
    AppData *ad = data;
    open_file_dialog(GTK_WINDOW(ad->window), "Choose an Image Folder", 
                             GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, on_folder_selected, ad);
}

static void on_prev(GtkButton *btn, gpointer data) {
    AppData *ad = data;
    if (!ad->current_image) return;
    ad->current_image = ad->current_image->prev ? ad->current_image->prev : g_list_last(ad->current_image);
    display_current(ad);
}

static void on_next(GtkButton *btn, gpointer data) {
    AppData *ad = data;
    if (!ad->current_image) return;
    ad->current_image = ad->current_image->next ? ad->current_image->next : g_list_first(ad->current_image);
    display_current(ad);
}

static void activate(GtkApplication *app, gpointer user_data) {
    AppData *ad = g_new0(AppData, 1);
    
    ad->window = create_standard_window(app, "Media Player", 800, 600);
    GtkWidget *vbox = create_padded_box(GTK_ORIENTATION_VERTICAL, 10, 10);

    ad->image_widget = gtk_image_new();
    gtk_widget_set_vexpand(ad->image_widget, TRUE);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
    gtk_widget_set_halign(hbox, GTK_ALIGN_CENTER);

    GtkWidget *btn_open = gtk_button_new_with_label("Open Folder");
    GtkWidget *btn_prev = gtk_button_new_with_label("◀ Previous");
    GtkWidget *btn_next = gtk_button_new_with_label("Next ▶");

    g_signal_connect(btn_open, "clicked", G_CALLBACK(on_open_folder_clicked), ad);
    g_signal_connect(btn_prev, "clicked", G_CALLBACK(on_prev), ad);
    g_signal_connect(btn_next, "clicked", G_CALLBACK(on_next), ad);

    gtk_box_append(GTK_BOX(hbox), btn_prev);
    gtk_box_append(GTK_BOX(hbox), btn_open);
    gtk_box_append(GTK_BOX(hbox), btn_next);
    
    gtk_box_append(GTK_BOX(vbox), ad->image_widget);
    gtk_box_append(GTK_BOX(vbox), hbox);

    gtk_window_set_child(GTK_WINDOW(ad->window), vbox);
    gtk_window_present(GTK_WINDOW(ad->window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("companion.virus.MediaPlayer", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}