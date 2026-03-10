#include <assert.h>
#include <gtk/gtk.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// convert a number to its ascii value
char itos(int nb) {
    return (nb >= 0 && nb <= 9) ? (nb + '0') : ((nb - 10) + 'A');
}

// convert an ascii character to its number value
int stoi(char c) {
    return (c >= '0' && c <= '9') ? (c - '0') : ((c - 'A') + 10);
}

size_t sizeNeeded(int num, int base) {
    if (num == 0) return 2;
    size_t len = floor(log(abs(num)) / log(base)) + 1;

    return len + 1;
}

char *reverse(char *s) {
    int n = strlen(s);
    char *res = malloc((n + 1) * sizeof(char));
    if (res == NULL) {
        fprintf(stderr, "Allocation failed\n");
        return NULL;
    }

    int j = 0;
    for (int i = n - 1; i >= 0; i--) {
        res[j] = s[i];
        j++;
    }

    res[n] = '\0';

    return res;
}

// convert a decimal number to the defined base
char *convertDecimalTo(int number, int base) {
    if (number == 0) {
        char *res = malloc(2);
        if (res == NULL) {
            fprintf(stderr, "Allocation failed\n");
            return NULL;
        }

        res[0] = '0';
        res[1] = '\0';

        return res;
    }

    if (base < 2 || base > 36) {
        fprintf(stderr, "Invalid base\n");
        return NULL;
    }

    char *arr = malloc(sizeNeeded(number, base) * sizeof(char));
    if (arr == NULL) {
        fprintf(stderr, "Allocation failed\n");
        return NULL;
    }

    int pos = 0;
    while (number > 0) {
        char res = itos(number % base);
        arr[pos] = res;
        number /= base;
        pos++;
    }

    arr[pos] = '\0';

    char *reversed = reverse(arr);
    free(arr);

    return reversed;
}

// convert a number from any base to decimal
int convertDecimalFrom(const char *digits, int base) {
    if (base < 2 || base > 36) {
        fprintf(stderr, "Invalid base\n");
        return -1;
    }

    int n = 0;
    size_t len = strlen(digits);
    for (int d = 0; d < len; d++) {
        int val = stoi(digits[d]);
        if (val >= base) {
            fprintf(stderr, "Invalid digit for base\n");
            return -1;
        }

        n = (base * n) + val;
    }

    return n;
}

// graphical interface

typedef struct {
    GtkWidget *entry;
    GtkWidget *combo;
    GtkWidget *spin;
    GtkWidget *label;
} ConverterWidgets;

static void do_conversion(ConverterWidgets *cw) {
    const char *text = gtk_editable_get_text(GTK_EDITABLE(cw->entry));
    guint active = gtk_drop_down_get_selected(GTK_DROP_DOWN(cw->combo));
    int base = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(cw->spin));

    if (strlen(text) == 0) {
        gtk_label_set_text(GTK_LABEL(cw->label), "Waiting for input...");
        return;
    }

    if (active == 0) {  // Decimal → Base
        char *res = convertDecimalTo(atoi(text), base);
        gtk_label_set_text(GTK_LABEL(cw->label), res ? res : "Error");
        free(res);
    } else {  // Base → Decimal
        int number = convertDecimalFrom(text, base);
        if (number != -1) {
            char result[64];
            snprintf(result, sizeof(result), "%d", number);
            gtk_label_set_text(GTK_LABEL(cw->label), result);
        } else {
            gtk_label_set_text(GTK_LABEL(cw->label), "Invalid input");
        }
    }
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Bases Converter");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, -1);

    ConverterWidgets *cw = g_new0(ConverterWidgets, 1);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_start(vbox, 20);
    gtk_widget_set_margin_end(vbox, 20);
    gtk_widget_set_margin_top(vbox, 20);
    gtk_widget_set_margin_bottom(vbox, 20);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    const char *options[] = {"Decimal → Base", "Base → Decimal", NULL};
    cw->combo = gtk_drop_down_new_from_strings(options);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(cw->combo), 0);
    cw->entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(cw->entry), "Enter value...");
    cw->spin = gtk_spin_button_new_with_range(2, 36, 1);
    cw->label = gtk_label_new("Result :");
    gtk_widget_add_css_class(cw->label, "title-2");

    gtk_box_append(GTK_BOX(vbox), cw->combo);
    gtk_box_append(GTK_BOX(vbox), cw->entry);
    gtk_box_append(GTK_BOX(vbox), gtk_label_new("Target Base:"));
    gtk_box_append(GTK_BOX(vbox), cw->spin);
    gtk_box_append(GTK_BOX(vbox), cw->label);

    // Convert while typing
    g_signal_connect_swapped(cw->entry, "changed", G_CALLBACK(do_conversion),
                             cw);
    g_signal_connect_swapped(cw->combo, "selected", G_CALLBACK(do_conversion),
                             cw);
    g_signal_connect_swapped(cw->spin, "value-changed",
                             G_CALLBACK(do_conversion), cw);

    g_signal_connect_swapped(window, "destroy", G_CALLBACK(g_free), cw);

    gtk_window_present(GTK_WINDOW(window));
}

int testConverter() {
    char *res;
    res = convertDecimalTo(282, 2);
    assert(res != NULL);
    assert(strcmp(res, "100011010") == 0);
    free(res);

    res = convertDecimalTo(282, 8);
    assert(res != NULL);
    assert(strcmp(res, "432") == 0);
    free(res);

    res = convertDecimalTo(282, 16);
    assert(res != NULL);
    assert(strcmp(res, "11A") == 0);
    free(res);

    res = convertDecimalTo(282, 36);
    assert(res != NULL);
    assert(strcmp(res, "7U") == 0);
    free(res);

    assert(convertDecimalTo(282, 1) == NULL);  // Invalid base

    assert(convertDecimalFrom("100011010", 2) == 282);
    assert(convertDecimalFrom("432", 8) == 282);
    assert(convertDecimalFrom("11A", 16) == 282);
    assert(convertDecimalFrom("7U", 36) == 282);
    assert(convertDecimalFrom("100011010", 1) == -1);  // Invalid base

    return 0;
}

int main(int argc, char **argv) {
    testConverter();

    GtkApplication *app = gtk_application_new("companion.virus.converter",
                                              G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}
