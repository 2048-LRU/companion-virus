#include "common/gui_utils.h"

#include <gtk/gtk.h>

typedef struct {
    FileSelectedCallback callback;
    gpointer user_data;
} DialogContext;

static void on_dialog_response(GtkNativeDialog *dialog, gint response,
                               gpointer data) {
    DialogContext *ctx = (DialogContext *)data;

    if (response == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        GFile *file = gtk_file_chooser_get_file(chooser);
        if (file) {
            char *path = g_file_get_path(file);
            if (ctx->callback) {
                ctx->callback(path, ctx->user_data);
            }
            g_free(path);
            g_object_unref(file);
        }
    }
    g_object_unref(dialog);
    g_free(ctx);
}

void open_file_dialog(GtkWindow *parent, const char *title,
                      GtkFileChooserAction action,
                      FileSelectedCallback callback, gpointer user_data) {
    GtkFileChooserNative *dialog = gtk_file_chooser_native_new(
        title, parent, action, "_Ouvrir", "_Annuler");

    DialogContext *ctx = g_new0(DialogContext, 1);
    ctx->callback = callback;
    ctx->user_data = user_data;

    g_signal_connect(dialog, "response", G_CALLBACK(on_dialog_response), ctx);
    gtk_native_dialog_show(GTK_NATIVE_DIALOG(dialog));
}

GtkWidget *create_standard_window(GtkApplication *app, const char *title,
                                  int width, int height) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), title);
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);
    return window;
}

GtkWidget *create_padded_box(GtkOrientation orientation, int spacing,
                             int margin) {
    GtkWidget *box = gtk_box_new(orientation, spacing);
    gtk_widget_set_margin_top(box, margin);
    gtk_widget_set_margin_bottom(box, margin);
    gtk_widget_set_margin_start(box, margin);
    gtk_widget_set_margin_end(box, margin);
    return box;
}
