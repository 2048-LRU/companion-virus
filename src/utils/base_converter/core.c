#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/base_converter.h"

char int_to_string_char(int nb) {
    return (nb >= 0 && nb <= 9) ? (nb + '0') : ((nb - 10) + 'A');
}

int string_char_to_int(char c) {
    return (c >= '0' && c <= '9') ? (c - '0') : ((c - 'A') + 10);
}

size_t size_needed(int num, int base) {
    if (num == 0) return 2;
    size_t len = floor(log(abs(num)) / log(base)) + 1;
    return len + 1;
}

char *reverse_string(char *s) {
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

char *convert_decimal_to(int number, int base) {
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

    char *arr = malloc(size_needed(number, base) * sizeof(char));
    if (arr == NULL) {
        fprintf(stderr, "Allocation failed\n");
        return NULL;
    }

    int pos = 0;
    while (number > 0) {
        char res = int_to_string_char(number % base);
        arr[pos] = res;
        number /= base;
        pos++;
    }

    arr[pos] = '\0';
    char *reversed = reverse_string(arr);
    free(arr);
    return reversed;
}

int convert_to_decimal(const char *digits, int base) {
    if (base < 2 || base > 36) {
        fprintf(stderr, "Invalid base\n");
        return -1;
    }

    int n = 0;
    size_t len = strlen(digits);
    for (int d = 0; d < len; d++) {
        int val = string_char_to_int(digits[d]);
        if (val >= base) {
            fprintf(stderr, "Invalid digit for base\n");
            return -1;
        }
        n = (base * n) + val;
    }

    return n;
}

void free_converted(char *ptr) {
    if (ptr) free(ptr);
}
