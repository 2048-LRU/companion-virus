#include "utils/num_generator.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_generate_random_in_range() {
    seed_generator(42);
    for (int i = 0; i < 100; i++) {
        int result = generate_random(10, 20);
        assert(result >= 10 && result <= 20);
    }
}

void test_generate_random_single_value() {
    int result = generate_random(5, 5);
    assert(result == 5);
}

void test_generate_random_large_range() {
    seed_generator(123);
    for (int i = 0; i < 50; i++) {
        int result = generate_random(0, 1000000);
        assert(result >= 0 && result <= 1000000);
    }
}

void test_generate_random_negative() {
    seed_generator(99);
    for (int i = 0; i < 50; i++) {
        int result = generate_random(-100, -1);
        assert(result >= -100 && result <= -1);
    }
}

void test_generate_random_float_range() {
    seed_generator(55);
    for (int i = 0; i < 50; i++) {
        float result = generate_random_float();
        assert(result >= 0.0f && result <= 1.0f);
    }
}

void test_seed_reproducibility() {
    seed_generator(12345);
    int r1 = generate_random(1, 100);
    int r2 = generate_random(1, 100);
    int r3 = generate_random(1, 100);

    seed_generator(12345);
    assert(r1 == generate_random(1, 100));
    assert(r2 == generate_random(1, 100));
    assert(r3 == generate_random(1, 100));
}

void test_different_seeds() {
    seed_generator(111);
    int seq1_val1 = generate_random(1, 1000);
    int seq1_val2 = generate_random(1, 1000);

    seed_generator(222);
    int seq2_val1 = generate_random(1, 1000);
    int seq2_val2 = generate_random(1, 1000);

    assert(seq1_val1 != seq2_val1 || seq1_val2 != seq2_val2);
}

int main() {
    test_generate_random_in_range();
    test_generate_random_single_value();
    test_generate_random_large_range();
    test_generate_random_negative();
    test_generate_random_float_range();
    test_seed_reproducibility();
    test_different_seeds();

    printf("num_generator: all tests passed\n");
    return 0;
}
