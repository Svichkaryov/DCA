#include "stdafx.h"
#include "Simon.h"


inline uint16_t Simon::S(uint16_t state, int distance)
{
	return (distance > 0) ?
		((state << distance) | (state >> (16 - distance))) & WORD_MASK :
		((state >> (-distance)) | (state << (16 + distance))) & WORD_MASK;
}

inline uint16_t Simon::f(uint16_t state)
{
	return (S(state, 1) & S(state, 8)) ^ S(state, 2);
}

void Simon::encrypt_block(const uint16_t plaintext[2], const uint16_t key[4],
	uint16_t ciphertext[2])
{
	partial_encrypt_block(plaintext, key, ciphertext, ROUNDS);
}

void Simon::decrypt_block(const uint16_t ciphertext[2], const uint16_t key[4],
	uint16_t plaintext[2])
{
	partial_decrypt_block(ciphertext, key, plaintext, ROUNDS);
}

void Simon::partial_encrypt_block(const uint16_t plaintext[2], const uint16_t key[4],
	uint16_t ciphertext[2], int rounds, int startInRound)
{
	uint16_t tmp;
	uint16_t key_schedule[ROUNDS];
	ciphertext[0] = plaintext[0];
	ciphertext[1] = plaintext[1];

	for (int i = 0; i < 4; ++i)
	{
		key_schedule[i] = key[i];
	}
	for (int i = 4; i < ROUNDS; ++i)
	{
		tmp = S(key_schedule[i - 1], -3);
		tmp ^= key_schedule[i - 3];
		tmp ^= S(tmp, -1);
		key_schedule[i] = key_schedule[i - 4] ^ tmp 
			^ z[CONST_J][(i - 4) % 62] ^ CONST_C;
	}

	for (int i = 0; i < rounds; ++i) 
	{
		tmp = ciphertext[1];
		ciphertext[1] = ciphertext[0] ^ f(ciphertext[1]) ^ key_schedule[startInRound + i];
		ciphertext[0] = tmp;
	}
}

void Simon::partial_decrypt_block(const uint16_t ciphertext[2], const uint16_t key[4],
	uint16_t plaintext[2], int rounds, int startInRound)
{
	uint16_t tmp;
	uint16_t key_schedule[ROUNDS];
	plaintext[0] = ciphertext[0];
	plaintext[1] = ciphertext[1];
	
	for (int i = 0; i < 4; ++i)
	{
		key_schedule[i] = key[i];
	}
	for (int i = 4; i < ROUNDS; ++i)
	{
		tmp = S(key_schedule[i - 1], -3);
		tmp ^= key_schedule[i - 3];
		tmp ^= S(tmp, -1);
		key_schedule[i] = key_schedule[i - 4] ^ tmp 
			^ z[CONST_J][(i - 4) % 62] ^ CONST_C;
	}

	for (int i = 0; i < rounds; ++i)
	{
		tmp = plaintext[0];
		plaintext[0] = plaintext[1] ^ f(plaintext[0]) ^ key_schedule[startInRound - i - 1];
		plaintext[1] = tmp;
	}
}