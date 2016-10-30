#ifndef MURMUR_H
#define MURMUR_H

#include <stdint.h>

void murmur_hash_32 (const void* key, int length, uint32_t seed, void* output);
void murmur_hash_128 (const void* key, int length, uint32_t seed, void* output);

#endif
