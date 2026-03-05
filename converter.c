#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

char *itos(int nb){
    int size = (int)((ceil(log10(nb))+1)*sizeof(char));
    char *nbtos = malloc(size);

    sprintf(nbtos, "%d", nb);

    return nbtos;
}

int sizeNeeded(int b1, int b2){
    char *b1_tostr = itos(b1);

    int len_b1 = strlen(b1_tostr);
    int size_convertion = len_b1 * log(b1)/log(b2);

    return size_convertion;
}

char *convertTo(int destination, int base){
    char *arr = malloc(sizeNeeded(base, destination));

    while(base > 0){
        char *res = itos(base % destination);
        printf("%s\n", res);
        strcat(arr, res);
        base = round(base / destination);
    }
    
    return arr;
}

int convertFrom(char *digits, int base){
    int n = 0;

    for(int d = 0; d < strlen(digits); d++){
        n = base * n + d;
    }

    return n;
}

// graphical interface
int main() {
    printf("%d\n", convertFrom("11012", 3));
    printf("%s\n", convertTo(7, 2));
    return 0;
}