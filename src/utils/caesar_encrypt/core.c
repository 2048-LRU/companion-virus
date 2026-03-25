#include <ctype.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/caesar_encrypt.h"

char caesar_cipher_char(int shift, char character) {
    if (!isalpha(character)) return character;
    char start = isupper(character) ? 'A' : 'a';
    return start + ((character - start + shift + 26) % 26);
}

char *caesar_cipher_string(const char *input, int shift) {
    size_t len = strlen(input);
    char *result = malloc((len + 1) * sizeof(char));
    if (result == NULL) return NULL;

    for (size_t i = 0; i < len; i++) {
        result[i] = caesar_cipher_char(shift, input[i]);
    }
    result[len] = '\0';
    return result;
}

void caesar_cipher_file(const char *input_file, int shift) {
    FILE *src = fopen(input_file, "r");
    if (!src) return;

    char *dir_path = g_path_get_dirname(input_file);
    const char *base_name = g_path_get_basename(input_file);
    char output_path[512];
    snprintf(output_path, sizeof(output_path), "%s/cypher_%s", dir_path,
             base_name);

    FILE *dst = fopen(output_path, "w");
    if (!dst) {
        fclose(src);
        return;
    }

    int c;
    while ((c = fgetc(src)) != EOF) {
        fputc(caesar_cipher_char(shift, c), dst);
    }

    fclose(src);
    fclose(dst);
}

void free_ciphered(char *ptr) {
    if (ptr) free(ptr);
}
