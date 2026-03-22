#include "utils/temp_converter.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define EPSILON 0.01f

int float_equal(float a, float b) { return fabsf(a - b) < EPSILON; }

void test_celsius_to_fahrenheit_freezing() {
    float result = celsius_to_fahrenheit(0);
    assert(float_equal(result, 32.0f));
}

void test_celsius_to_fahrenheit_boiling() {
    float result = celsius_to_fahrenheit(100);
    assert(float_equal(result, 212.0f));
}

void test_celsius_to_fahrenheit_negative() {
    float result = celsius_to_fahrenheit(-40);
    assert(float_equal(result, -40.0f));
}

void test_fahrenheit_to_celsius_freezing() {
    float result = fahrenheit_to_celsius(32);
    assert(float_equal(result, 0.0f));
}

void test_fahrenheit_to_celsius_boiling() {
    float result = fahrenheit_to_celsius(212);
    assert(float_equal(result, 100.0f));
}

void test_celsius_to_kelvin_freezing() {
    float result = celsius_to_kelvin(0);
    assert(float_equal(result, 273.15f));
}

void test_celsius_to_kelvin_absolute_zero() {
    float result = celsius_to_kelvin(-273.15f);
    assert(float_equal(result, 0.0f));
}

void test_kelvin_to_celsius() {
    float result = kelvin_to_celsius(273.15f);
    assert(float_equal(result, 0.0f));
}

void test_roundtrip_c_to_f() {
    float original = 20.5f;
    float fahrenheit = celsius_to_fahrenheit(original);
    float back = fahrenheit_to_celsius(fahrenheit);
    assert(float_equal(original, back));
}

void test_roundtrip_c_to_k() {
    float original = 50.0f;
    float kelvin = celsius_to_kelvin(original);
    float back = kelvin_to_celsius(kelvin);
    assert(float_equal(original, back));
}

void test_known_conversion() {
    float celsius_37 = celsius_to_fahrenheit(37);
    assert(float_equal(celsius_37, 98.6f));
}

int main() {
    test_celsius_to_fahrenheit_freezing();
    test_celsius_to_fahrenheit_boiling();
    test_celsius_to_fahrenheit_negative();
    test_fahrenheit_to_celsius_freezing();
    test_fahrenheit_to_celsius_boiling();
    test_celsius_to_kelvin_freezing();
    test_celsius_to_kelvin_absolute_zero();
    test_kelvin_to_celsius();
    test_roundtrip_c_to_f();
    test_roundtrip_c_to_k();
    test_known_conversion();

    printf("temp_converter: all tests passed\n");
    return 0;
}
