#include "stdafx.h"
#include "Simeck.h"


#define LROT16(x, r) (((x) << (r)) | ((x) >> (16 - (r))))

#define ROUND32(key, lft, rgt, tmp) do { \
    tmp = (lft); \
    lft = ((lft) & LROT16((lft), 5)) ^ LROT16((lft), 1) ^ (rgt) ^ (key); \
    rgt = (tmp); \
} while (0)


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
		ROUND32(keys[0], ciphertext[1], ciphertext[0], temp);

		constant &= 0xFFFC;
		constant |= sequence & 1;
		sequence >>= 1;
		ROUND32(constant, keys[1], keys[0], temp);

		temp = keys[1];
		keys[1] = keys[2];
		keys[2] = keys[3];
		keys[3] = temp;
	}
}


void Simeck::decrypt_block(const uint16_t ciphertext[2], const uint16_t key[4], uint16_t plaintext[2])
{

}
