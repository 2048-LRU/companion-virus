#include <gtk/gtk.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
int convertDecimalFrom(char *digits, int base) {
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

// graphical interface, example

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Bases converter");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_widget_show(window);
}

int testConverter(){
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

    assert(convertDecimalTo(282, 1) == NULL); // Invalid base

    assert(convertDecimalFrom("100011010", 2) == 282);
    assert(convertDecimalFrom("432", 8) == 282);
    assert(convertDecimalFrom("11A", 16) == 282);
    assert(convertDecimalFrom("7U", 36) == 282);
    assert(convertDecimalFrom("100011010", 1) == -1); // Invalid base

    return 0;
}

int main(int argc, char **argv) {
    testConverter();

    GtkApplication *app = gtk_application_new("companion.virus.converter",G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}
