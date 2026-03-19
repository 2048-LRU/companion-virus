#include "core/file_hasher.h"

#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *hash_file_sha256(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        return NULL;
    }

    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    unsigned char buffer[4096];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        SHA256_Update(&sha256, buffer, bytesRead);
    }

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_Final(hash, &sha256);
    fclose(file);

    char *result = malloc(SHA256_HEX_LENGTH);
    if (!result) return NULL;

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(result + (i * 2), "%02x", hash[i]);
    }

    return result;
}

char *hash_buffer_sha256(const unsigned char *buffer, size_t size) {
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, buffer, size);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_Final(hash, &sha256);

    char *result = malloc(SHA256_HEX_LENGTH);
    if (!result) return NULL;

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(result + (i * 2), "%02x", hash[i]);
    }

    return result;
}

void free_hash(char *ptr) {
    if (ptr) free(ptr);
}
