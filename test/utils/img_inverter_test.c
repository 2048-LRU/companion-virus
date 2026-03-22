#include "utils/img_inverter.h"

#include <assert.h>
#include <gd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void test_create_test_image() {
    gdImagePtr img = gdImageCreate(100, 100);
    assert(img != NULL);
    int white = gdImageColorAllocate(img, 255, 255, 255);
    int black = gdImageColorAllocate(img, 0, 0, 0);

    gdImageFilledRectangle(img, 0, 0, 50, 50, white);
    gdImageFilledRectangle(img, 50, 50, 100, 100, black);

    FILE *out = fopen("/tmp/test_image.png", "wb");
    assert(out != NULL);
    gdImagePng(img, out);
    fclose(out);
    gdImageDestroy(img);
}

void test_get_image_dimensions() {
    test_create_test_image();

    int width = 0, height = 0;
    get_image_dimensions("/tmp/test_image.png", &width, &height);

    assert(width == 100);
    assert(height == 100);

    unlink("/tmp/test_image.png");
}

void test_invert_image_basic() {
    test_create_test_image();

    invert_image_colors("/tmp/test_image.png", "/tmp/test_inverted.png");

    int width = 0, height = 0;
    get_image_dimensions("/tmp/test_inverted.png", &width, &height);

    assert(width == 100);
    assert(height == 100);

    unlink("/tmp/test_image.png");
    unlink("/tmp/test_inverted.png");
}

void test_invert_preserves_dimensions() {
    gdImagePtr img = gdImageCreate(200, 150);
    assert(img != NULL);
    int color = gdImageColorAllocate(img, 128, 128, 128);
    gdImageFilledRectangle(img, 0, 0, 200, 150, color);

    FILE *out = fopen("/tmp/test_dims.png", "wb");
    gdImagePng(img, out);
    fclose(out);
    gdImageDestroy(img);

    invert_image_colors("/tmp/test_dims.png", "/tmp/test_dims_inv.png");

    int w = 0, h = 0;
    get_image_dimensions("/tmp/test_dims_inv.png", &w, &h);
    assert(w == 200);
    assert(h == 150);

    unlink("/tmp/test_dims.png");
    unlink("/tmp/test_dims_inv.png");
}

int main() {
    test_get_image_dimensions();
    test_invert_image_basic();
    test_invert_preserves_dimensions();

    printf("img_inverter: all tests passed\n");
    return 0;
}
