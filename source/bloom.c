#include <stdlib.h>
#include <stdio.h>
#include <bloom.h>
#include <murmur.h>

void static inline bloom_bit_on (const uint8_t position, bloom_filter_t* filter)
{
	int offset =  position / (filter->array_length);
	int element = position % (filter->array_length);
	filter->bitarray[offset] |= 0x1 << element;
}

int static inline bloom_bit_check (const uint8_t position, const bloom_filter_t* filter)
{
	int offset =  position / (filter->array_length);
	int element = position % (filter->array_length);
	return (filter->bitarray[offset] &= 0x1 << element) > 0;
}


bloom_filter_t bloom_new(uint8_t array_length, uint8_t num_hashes)
{
	bloom_filter_t filter;
	filter.num_hashes = num_hashes;
	filter.array_length = array_length;
	filter.num_members = 0;
	filter.bitarray = malloc(array_length * sizeof(uint8_t));

	return filter;
}

void bloom_insert(bloom_filter_t* filter, void* element, uint8_t el_size)
{	
	uint8_t hash[4];

	for (int i = 0; i < filter->num_hashes; i++) 
	{
		murmur_hash_32(element, el_size, i, hash);		
		int index = hash[0] % filter->array_length;
		printf("Hash insert: %d\n", index);
		filter->bitarray[index] = 1;
	}

	filter->num_members++;
}

int bloom_query(bloom_filter_t* filter, void* element, uint8_t el_size)
{	
	uint8_t hash[4];
	uint8_t count = 0;

	for (int i = 0; i < filter->num_hashes; i++) 
	{
		murmur_hash_32(element, el_size, i, hash);
		int index = hash[0] % filter->array_length;
		printf("Hash query: %d\n", index);
		
		count += filter->bitarray[index];
	}
	printf("Query count: %d\n", count);

	if (count < filter->num_hashes) return 0;
	else return 1;
}
