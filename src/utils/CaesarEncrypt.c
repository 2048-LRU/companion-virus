#include <ctype.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    GtkWidget *window;
    GtkWidget *spin;
    GtkWidget *fbutton;
    char *filename;
    int dec;
} CypherWidgets;


char caesar(int dec, char s) {
    if (!isalpha(s)) return s;

    char start = isupper(s) ? 'A' : 'a';

    return start + ((tolower(s) - start + dec + 26) % 26);
}

void cypher_file(const char *src_name, int dec) {
    FILE *src = fopen(src_name, "r");

    if (!src) {
        g_print("Error opening input file\n");
        return;
    }

    char *dir_path = g_path_get_dirname(src_name);
    const char *base_name = g_path_get_basename(src_name);

    char output_path[512];
    snprintf(output_path, sizeof(output_path), "%s/cypher_%s", dir_path,
             base_name);
    FILE *dst = fopen(output_path, "w");

    if (!dst) {
        fclose(src);
        g_print("Error opening output file\n");
        return;
    }

    int c;
    while ((c = fgetc(src)) != EOF) {
        fputc(caesar(dec, c), dst);
    }

    fclose(src);
    fclose(dst);
    g_print("File encrypted -> %s\n", output_path);
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
    int dec = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(cw->spin));

    if (!cw->filename) {
        g_print("No file selected\n");
        return;
    }

    cypher_file(cw->filename, dec);
}

static void activate(GtkApplication *app, gpointer user_data) {
    CypherWidgets *cw = g_new0(CypherWidgets, 1);

    GtkWidget *window = gtk_application_window_new(app);
    cw->window = window;

    gtk_window_set_title(GTK_WINDOW(window), "Caesar Cypher");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_set_margin_top(box, 20);
    gtk_widget_set_margin_bottom(box, 20);
    gtk_widget_set_margin_start(box, 20);
    gtk_widget_set_margin_end(box, 20);

    gtk_window_set_child(GTK_WINDOW(window), box);

    GtkWidget *fbutton = gtk_button_new_with_label("Files...");
    GtkWidget *label = gtk_label_new("No file selected");
    GtkWidget *cypher = gtk_button_new_with_label("Cypher file");
    GtkWidget *spin = gtk_spin_button_new_with_range(1, 26, 1);
    cw->spin = spin;

    g_signal_connect(fbutton, "clicked", G_CALLBACK(open_file_dialog), cw);
    g_signal_connect(cypher, "clicked", G_CALLBACK(cypher_clicked), cw);
    gtk_label_set_text(GTK_LABEL(label), cw->filename);

    gtk_box_append(GTK_BOX(box), fbutton);
    gtk_box_append(GTK_BOX(box), label);
    gtk_box_append(GTK_BOX(box), gtk_label_new("shift : "));
    gtk_box_append(GTK_BOX(box), spin);
    gtk_box_append(GTK_BOX(box), cypher);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("companion.virus.caesar",
                                              G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);

    return status;
}