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
    for (int d = 0; d < strlen(digits); d++) {
        n = (base * n) + stoi(digits[d]);
    }

    return n;
}

// graphical interface
int main() {
    printf("%s\n", convertDecimalTo(282, 2));   // Result: 100011010
    printf("%s\n", convertDecimalTo(282, 8));   // Result: 432
    printf("%s\n", convertDecimalTo(282, 16));  // Result: 11A
    printf("%s\n", convertDecimalTo(282, 36));  // Result: 7U
    printf("%p\n", convertDecimalTo(282, 1));   // Error: Invalid base

    printf("\n");

    printf("%d\n", convertDecimalFrom("100011010", 2));  // Result: 282
    printf("%d\n", convertDecimalFrom("432", 8));        // Result: 282
    printf("%d\n", convertDecimalFrom("11A", 16));       // Result: 282
    printf("%d\n", convertDecimalFrom("7U", 36));        // Result: 282
    printf("%d\n", convertDecimalFrom("100011010", 1));  // Error: Invalid base

    return 0;
}
