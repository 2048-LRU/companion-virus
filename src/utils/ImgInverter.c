#include <ctype.h>
#include <gd.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

#define LRU_KEY 17

typedef struct {
    GtkWidget *window;
    GtkWidget *fbutton;
    char *filename;
} CypherWidgets;

void invert_image(const char *src, FILE *dest) {
    gdImagePtr img = gdImageCreateFromFile(src);
    if (!img) {
        printf("Failed to load image\n");
        return;
    }

    int width = gdImageSX(img);
    int height = gdImageSY(img);

    gdImagePtr out = gdImageCreateTrueColor(width, height);

    srand(time(NULL));

    for (int y = 0; y < height; y++) {
        int offset = (rand() % 60) - 20;
        for (int x = 0; x < width; x++) {

            int src_x = x + offset;
            if (src_x >= 0 && src_x < width) {
                int color = gdImageGetPixel(img, src_x, y);

                int r = gdTrueColorGetRed(color); 
                int g = gdTrueColorGetGreen(color);
                int b = gdTrueColorGetBlue(color);

                int new_color = gdImageColorAllocate(img, r ^ LRU_KEY, g ^ LRU_KEY << 2, b ^ LRU_KEY << 3);
                gdImageSetPixel(out, x, y, new_color);
            }
        }
    }

    gdImagePng(out, dest);

    gdImageDestroy(img);
    gdImageDestroy(out);
}

void cypher_file(const char *src_name) {
    FILE *src = fopen(src_name, "rb");

    if (!src) {
        g_print("Error opening input file\n");
        return;
    }

    char *dir_path = g_path_get_dirname(src_name);
    const char *base_name = g_path_get_basename(src_name);

    char output_path[512];
    snprintf(output_path, sizeof(output_path), "%s/LRU_inverted_%s", dir_path,
             base_name);
    FILE *dst = fopen(output_path, "wb");

    if (!dst) {
        fclose(src);
        g_print("Error opening output file\n");
        return;
    }

    // logique inversion
    invert_image(src_name, dst);

    fclose(src);
    fclose(dst);
    g_print("Image Inverted -> %s\n", output_path);
}

static void file_dialog_callback(GtkNativeDialog *dialog, gint response,
                                 gpointer data) {
    CypherWidgets *cw = data;

    if (response == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        GFile *file = gtk_file_chooser_get_file(chooser);

        if (file) {
            g_free(cw->filename);
            cw->filename = g_file_get_path(file);
            g_print("Selected: %s\n", cw->filename);
            g_object_unref(file);
        }
    }

    g_object_unref(dialog);
}

static void open_file_dialog(GtkButton *button, gpointer data) {
    CypherWidgets *cw = data;

    GtkFileChooserNative *dialog = gtk_file_chooser_native_new(
        "Open File", GTK_WINDOW(cw->window), GTK_FILE_CHOOSER_ACTION_OPEN,
        "_Open", "_Cancel");

    g_signal_connect(dialog, "response", G_CALLBACK(file_dialog_callback), cw);
    gtk_native_dialog_show(GTK_NATIVE_DIALOG(dialog));
}

static void cypher_clicked(GtkButton *button, gpointer data) {
    CypherWidgets *cw = data;
    if (!cw->filename) {
        g_print("No file selected\n");
        return;
    }

    cypher_file(cw->filename);
}

static void activate(GtkApplication *app, gpointer user_data) {
    CypherWidgets *cw = g_new0(CypherWidgets, 1);

    GtkWidget *window = gtk_application_window_new(app);
    cw->window = window;

    gtk_window_set_title(GTK_WINDOW(window), "Image Inverter");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_set_margin_top(box, 20);
    gtk_widget_set_margin_bottom(box, 20);
    gtk_widget_set_margin_start(box, 20);
    gtk_widget_set_margin_end(box, 20);

    gtk_window_set_child(GTK_WINDOW(window), box);

    GtkWidget *fbutton = gtk_button_new_with_label("Images...");
    GtkWidget *label = gtk_label_new("No file selected");
    GtkWidget *cypher = gtk_button_new_with_label("Invert Image");

    g_signal_connect(fbutton, "clicked", G_CALLBACK(open_file_dialog), cw);
    g_signal_connect(cypher, "clicked", G_CALLBACK(cypher_clicked), cw);
    gtk_label_set_text(GTK_LABEL(label), cw->filename);

    gtk_box_append(GTK_BOX(box), fbutton);
    gtk_box_append(GTK_BOX(box), label);
    gtk_box_append(GTK_BOX(box), cypher);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("companion.virus.img_invert",
                                              G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}