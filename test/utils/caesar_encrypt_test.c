#include "utils/caesar_encrypt.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_caesar_cipher_char_lowercase() {
    assert(caesar_cipher_char(1, 'a') == 'b');
    assert(caesar_cipher_char(1, 'z') == 'a');
    assert(caesar_cipher_char(3, 'a') == 'd');
}

void test_caesar_cipher_char_uppercase() {
    assert(caesar_cipher_char(1, 'A') == 'B');
    assert(caesar_cipher_char(1, 'Z') == 'A');
    assert(caesar_cipher_char(3, 'A') == 'D');
}

void test_caesar_cipher_char_non_alpha() {
    assert(caesar_cipher_char(1, '1') == '1');
    assert(caesar_cipher_char(1, ' ') == ' ');
    assert(caesar_cipher_char(1, '!') == '!');
}

void test_caesar_cipher_string_simple() {
    char *result = caesar_cipher_string("abc", 1);
    assert(result != NULL);
    assert(strcmp(result, "bcd") == 0);
    free_ciphered(result);
}

void test_caesar_cipher_string_wrap() {
    char *result = caesar_cipher_string("xyz", 1);
    assert(result != NULL);
    assert(strcmp(result, "yza") == 0);
    free_ciphered(result);
}

void test_caesar_cipher_string_mixed() {
    char *result = caesar_cipher_string("Hello, World!", 1);
    assert(result != NULL);
    assert(strcmp(result, "Ifmmp, Xpsme!") == 0);
    free_ciphered(result);
}

void test_caesar_cipher_string_negative() {
    char *result = caesar_cipher_string("bcd", -1);
    assert(result != NULL);
    assert(strcmp(result, "abc") == 0);
    free_ciphered(result);
}

void test_caesar_cipher_string_zero() {
    char *result = caesar_cipher_string("test", 0);
    assert(result != NULL);
    assert(strcmp(result, "test") == 0);
    free_ciphered(result);
}

int main() {
    test_caesar_cipher_char_lowercase();
    test_caesar_cipher_char_uppercase();
    test_caesar_cipher_char_non_alpha();
    test_caesar_cipher_string_simple();
    test_caesar_cipher_string_wrap();
    test_caesar_cipher_string_mixed();
    test_caesar_cipher_string_negative();
    test_caesar_cipher_string_zero();

    printf("caesar_encrypt: all tests passed\n");
    return 0;
}
