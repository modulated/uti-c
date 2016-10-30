#ifndef BLOOM_H
#define BLOOM_H

#include <stdint.h>

typedef struct {
	uint8_t array_length;
	uint8_t num_members;
	uint8_t num_hashes;
	uint8_t* bitarray;
} bloom_filter_t;

bloom_filter_t bloom_new(uint8_t array_length, uint8_t num_hashes);
void bloom_insert(bloom_filter_t* filter, void* element, uint8_t el_size);
int bloom_query(bloom_filter_t* filter, void* element, uint8_t el_size);

#endif

