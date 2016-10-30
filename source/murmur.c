#include <murmur.h>

#define BIG_CONSTANT(x) (x##LLU)
#define GETBLOCK(p, i) (p[i])
static inline uint32_t rotl32 (uint32_t x, uint8_t r) 
{
	return (x << r) | (x >> (32 - r));
}

static inline uint64_t rotl64 ( uint64_t x, int8_t r )
{
	return (x << r) | (x >> (64 - r));
}

static inline uint32_t final_mix_32 (uint32_t h)
{
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;

	return h;
}

static inline uint64_t final_mix_64 ( uint64_t k )
{
	k ^= k >> 33;
	k *= BIG_CONSTANT(0xff51afd7ed558ccd);
	k ^= k >> 33;
	k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
	k ^= k >> 33;

	return k;
}


void murmur_hash_32 (const void* key, int length, uint32_t seed, void* out)
{
	const uint8_t* data = (const uint8_t*)key;
	const int nblocks = length / 4;
	int i;

	uint32_t h1 = seed;

	uint32_t c1 = 0xcc9e2d51;
	uint32_t c2 = 0x1b873593;


	// body
	const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);

	for(i = -nblocks; i; i++)
	{
		uint32_t k1 = GETBLOCK(blocks,i);

		k1 *= c1;
		k1 = rotl32(k1,15);
		k1 *= c2;

		h1 ^= k1;
		h1 = rotl32(h1,13); 
		h1 = h1 * 5+0xe6546b64;
	}


	// tail
	const uint8_t * tail = (const uint8_t*)(data + nblocks*4);

	uint32_t k1 = 0;

	switch(length & 3)
	{
	case 3: k1 ^= tail[2] << 16;
	case 2: k1 ^= tail[1] << 8;
	case 1: k1 ^= tail[0];
		k1 *= c1; k1 = rotl32(k1,15); k1 *= c2; h1 ^= k1;
	};

	// finalization
	h1 ^= length;

	h1 = final_mix_32(h1);

	*(uint32_t*)out = h1;
}


void murmur_hash_128 (const void* key, const int length, const uint32_t seed, void* out)
{
	const uint8_t * data = (const uint8_t*)key;
	const int nblocks = length / 16;
	int i;

	uint64_t h1 = seed;
	uint64_t h2 = seed;

	uint64_t c1 = BIG_CONSTANT(0x87c37b91114253d5);
	uint64_t c2 = BIG_CONSTANT(0x4cf5ad432745937f);


	// body
	const uint64_t * blocks = (const uint64_t *)(data);

	for(i = 0; i < nblocks; i++)
	{
	uint64_t k1 = GETBLOCK(blocks,i*2+0);
	uint64_t k2 = GETBLOCK(blocks,i*2+1);

	k1 *= c1; k1  = rotl64(k1,31); k1 *= c2; h1 ^= k1;

	h1 = rotl64(h1,27); h1 += h2; h1 = h1*5+0x52dce729;

	k2 *= c2; k2  = rotl64(k2,33); k2 *= c1; h2 ^= k2;

	h2 = rotl64(h2,31); h2 += h1; h2 = h2*5+0x38495ab5;
	}


	// tail
	const uint8_t * tail = (const uint8_t*)(data + nblocks*16);

	uint64_t k1 = 0;
	uint64_t k2 = 0;

	switch(length & 15)
	{
	case 15: k2 ^= (uint64_t)(tail[14]) << 48;
	case 14: k2 ^= (uint64_t)(tail[13]) << 40;
	case 13: k2 ^= (uint64_t)(tail[12]) << 32;
	case 12: k2 ^= (uint64_t)(tail[11]) << 24;
	case 11: k2 ^= (uint64_t)(tail[10]) << 16;
	case 10: k2 ^= (uint64_t)(tail[ 9]) << 8;
	case  9: k2 ^= (uint64_t)(tail[ 8]) << 0;
		k2 *= c2; k2  = rotl64(k2,33); k2 *= c1; h2 ^= k2;

	case  8: k1 ^= (uint64_t)(tail[ 7]) << 56;
	case  7: k1 ^= (uint64_t)(tail[ 6]) << 48;
	case  6: k1 ^= (uint64_t)(tail[ 5]) << 40;
	case  5: k1 ^= (uint64_t)(tail[ 4]) << 32;
	case  4: k1 ^= (uint64_t)(tail[ 3]) << 24;
	case  3: k1 ^= (uint64_t)(tail[ 2]) << 16;
	case  2: k1 ^= (uint64_t)(tail[ 1]) << 8;
	case  1: k1 ^= (uint64_t)(tail[ 0]) << 0;
		k1 *= c1; k1  = rotl64(k1,31); k1 *= c2; h1 ^= k1;
	};


	// finalization
	h1 ^= length; h2 ^= length;

	h1 += h2;
	h2 += h1;

	h1 = final_mix_64(h1);
	h2 = final_mix_64(h2);

	h1 += h2;
	h2 += h1;

	((uint64_t*)out)[0] = h1;
	((uint64_t*)out)[1] = h2;
}
