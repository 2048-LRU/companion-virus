#include "utils/file_hasher.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void test_hash_buffer_empty() {
    unsigned char buffer[] = "";
    char *result = hash_buffer_sha256(buffer, 0);
    assert(result != NULL);
    assert(strlen(result) == 64);
    free_hash(result);
}

void test_hash_buffer_simple() {
    unsigned char buffer[] = "hello";
    char *result = hash_buffer_sha256(buffer, 5);
    assert(result != NULL);
    assert(strlen(result) == 64);
    free_hash(result);
}

void test_hash_buffer_consistency() {
    unsigned char buffer[] = "test";
    char *result1 = hash_buffer_sha256(buffer, 4);
    char *result2 = hash_buffer_sha256(buffer, 4);
    assert(strcmp(result1, result2) == 0);
    free_hash(result1);
    free_hash(result2);
}

void test_hash_file_basic() {
    FILE *temp = fopen("/tmp/test_hash.txt", "w");
    assert(temp != NULL);
    fprintf(temp, "test content");
    fclose(temp);

    char *result = hash_file_sha256("/tmp/test_hash.txt");
    assert(result != NULL);
    assert(strlen(result) == 64);
    free_hash(result);

    unlink("/tmp/test_hash.txt");
}

void test_hash_file_empty() {
    FILE *temp = fopen("/tmp/test_empty.txt", "w");
    assert(temp != NULL);
    fclose(temp);

    char *result = hash_file_sha256("/tmp/test_empty.txt");
    assert(result != NULL);
    assert(strlen(result) == 64);
    free_hash(result);

    unlink("/tmp/test_empty.txt");
}

void test_hash_file_consistency() {
    FILE *temp = fopen("/tmp/test_consistency.txt", "w");
    assert(temp != NULL);
    fprintf(temp, "consistent");
    fclose(temp);

    char *result1 = hash_file_sha256("/tmp/test_consistency.txt");
    char *result2 = hash_file_sha256("/tmp/test_consistency.txt");
    assert(strcmp(result1, result2) == 0);
    free_hash(result1);
    free_hash(result2);

    unlink("/tmp/test_consistency.txt");
}

void test_hash_different_content() {
    FILE *temp1 = fopen("/tmp/test_diff1.txt", "w");
    fprintf(temp1, "content1");
    fclose(temp1);

    FILE *temp2 = fopen("/tmp/test_diff2.txt", "w");
    fprintf(temp2, "content2");
    fclose(temp2);

    char *result1 = hash_file_sha256("/tmp/test_diff1.txt");
    char *result2 = hash_file_sha256("/tmp/test_diff2.txt");
    assert(strcmp(result1, result2) != 0);
    free_hash(result1);
    free_hash(result2);

    unlink("/tmp/test_diff1.txt");
    unlink("/tmp/test_diff2.txt");
}

int main() {
    test_hash_buffer_empty();
    test_hash_buffer_simple();
    test_hash_buffer_consistency();
    test_hash_file_basic();
    test_hash_file_empty();
    test_hash_file_consistency();
    test_hash_different_content();

    printf("file_hasher: all tests passed\n");
    return 0;
}
