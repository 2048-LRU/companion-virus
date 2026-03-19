#include <stdlib.h>
#include <time.h>

#include "utils/num_generator.h"

int generate_random(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

float generate_random_float(void) { return (float)rand() / (float)RAND_MAX; }

void seed_generator(unsigned int seed) { srand(seed); }
