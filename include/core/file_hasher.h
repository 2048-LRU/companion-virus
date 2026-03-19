#pragma once

#include <stddef.h>

#define SHA256_HEX_LENGTH 65  /* 64 hex chars + null terminator */

/* Compute SHA256 hash of a file, returns hex string */
char *hash_file_sha256(const char *filename);

/* Compute SHA256 hash of memory buffer, returns hex string */
char *hash_buffer_sha256(const unsigned char *buffer, size_t size);

void free_hash(char *ptr);