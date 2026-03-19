#pragma once

char caesar_cipher_char(int shift, char character);

void caesar_cipher_file(const char *input_file, int shift);

char *caesar_cipher_string(const char *input, int shift);

void free_ciphered(char *ptr);