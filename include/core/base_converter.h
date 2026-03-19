#pragma once

#include <stdlib.h>

char *convert_decimal_to(int number, int base);

int convert_to_decimal(const char *str, int base);

char int_to_string_char(int nb);

int string_char_to_int(char c);

/* Helper: calculate size needed for conversion */
size_t size_needed(int num, int base);

char *reverse_string(char *s);

void free_converted(char *ptr);