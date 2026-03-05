#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// convert a  number to its ascii val
char itos(int nb) {
    return (nb >= 0 && nb <= 9) ? (char)(nb + '0') : (char)(nb - 10 + 'A');
}

char *reverse(char *s) {
    int n = strlen(s);
    char *res = (char *)malloc((n + 1) * sizeof(char));

    int j = 0;

    for (int i = n - 1; i >= 0; i--) {
        res[j] = s[i];
        j++;
    }

    res[n] = '\0';
    return res;
}

int sizeNeeded(int b1, int b2) {
    int len_b1 = floor(log10(abs(b1))) + 1;
    int size_conversion = len_b1 * log(b1) / log(b2);

    return size_conversion;
}

// convert a decimal number to the defined base
char *convertTo(int number, int base) {
    char *arr = malloc(sizeNeeded(base, number));
    printf("%ld", sizeof(arr));
    while (number > 0) {
        char res = itos(number % base);
        sprintf(arr++, "%c", res);
        number /= base;
    }

    *arr = '\0';
    return arr;
}

// convert a number from any base to decimal
int convertFrom(char *digits, int base) {
    int n = 0;

    for (int d = 0; d < strlen(digits); d++) {
        n = base * n + d;
    }

    return n;
}

// graphical interface
int main() {
    // printf("%d\n", convertFrom("1102A", 16));
    printf("%s\n", convertTo(282, 16));
    printf("%c\n", itos(5));
    return 0;
}