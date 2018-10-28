#include "stdafx.h"
#include "Simeck.h"


#define LROT16(x, r) (((x) << (r)) | ((x) >> (16 - (r))))

#define ROUND32(key, lft, rgt, tmp) do { \
    tmp = (lft); \
    lft = ((lft) & LROT16((lft), 5)) ^ LROT16((lft), 1) ^ (rgt) ^ (key); \
    rgt = (tmp); \
} while (0)


inline void Simeck::round_func(uint16_t& x, uint16_t& y, const uint16_t key)
{
	uint16_t tmp = x;
	x = (x & ( (x << 5) | (x >> 11))) ^ (x << 1 | x >> 15) ^ y ^ key;
	y = tmp;
}

inline void Simeck::reverse_round_func(uint16_t& x, uint16_t& y, const uint16_t key)
{
	y ^= x;
	y = (y >> 2) | (y << 14);
	x ^= key;
	x -= y;
	x = (x << 7) | (x >> 9);
}


void Simeck::encrypt_block(const uint16_t plaintext[2], const uint16_t key[4], uint16_t ciphertext[2])
{
	int idx;

	uint16_t keys[4] = { key[0], key[1], key[2], key[3] };
	ciphertext[0] = plaintext[0];
	ciphertext[1] = plaintext[1];
	uint16_t temp;

	uint16_t constant = 0xFFFC;
	uint32_t sequence = 0x9A42BB1F;

	for (idx = 0; idx < 32; ++idx) 
	{
		round_func(ciphertext[1], ciphertext[0], keys[0]);

		constant &= 0xFFFC;
		constant |= sequence & 1;
		sequence >>= 1;
		round_func(keys[1], keys[0], constant);

		temp = keys[1];
		keys[1] = keys[2];
		keys[2] = keys[3];
		keys[3] = temp;
	}
}


void Simeck::decrypt_block(const uint16_t ciphertext[2], const uint16_t key[4], uint16_t plaintext[2])
{

}
