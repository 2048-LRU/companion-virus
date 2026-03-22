#include "utils/base_converter.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_decimal_to_binary() {
    char *result = convert_decimal_to(5, 2);
    assert(result != NULL);
    assert(strcmp(result, "101") == 0);
    free_converted(result);
}

void test_decimal_to_hex() {
    char *result = convert_decimal_to(255, 16);
    assert(result != NULL);
    assert(strcmp(result, "FF") == 0);
    free_converted(result);
}

void test_decimal_to_octal() {
    char *result = convert_decimal_to(8, 8);
    assert(result != NULL);
    assert(strcmp(result, "10") == 0);
    free_converted(result);
}

void test_to_decimal_from_binary() {
    int result = convert_to_decimal("101", 2);
    assert(result == 5);
}

void test_to_decimal_from_hex() {
    int result = convert_to_decimal("FF", 16);
    assert(result == 255);
}

void test_int_to_string_char() {
    assert(int_to_string_char(0) == '0');
    assert(int_to_string_char(9) == '9');
    assert(int_to_string_char(10) == 'A');
    assert(int_to_string_char(15) == 'F');
}

void test_string_char_to_int() {
    assert(string_char_to_int('0') == 0);
    assert(string_char_to_int('9') == 9);
    assert(string_char_to_int('A') == 10);
    assert(string_char_to_int('F') == 15);
}

void test_reverse_string() {
    char test[] = "hello";
    char *result = reverse_string(test);
    assert(strcmp(result, "olleh") == 0);
    free(result);
}

int main() {
    test_decimal_to_binary();
    test_decimal_to_hex();
    test_decimal_to_octal();
    test_to_decimal_from_binary();
    test_to_decimal_from_hex();
    test_int_to_string_char();
    test_string_char_to_int();
    test_reverse_string();

    printf("base_converter: all tests passed\n");
    return 0;
}
