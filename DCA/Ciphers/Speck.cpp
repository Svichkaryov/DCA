#include "stdafx.h"
#include "Speck.h"
#include "windows.h"


// SPECK_32_64

inline void Speck::roundFunc(uint16_t& x, uint16_t& y, const uint16_t key)
{
	x = (x >> 7) | (x << 9);
	x += y;
	x ^= key;
	y = (y << 2) | (y >> 14);
	y ^= x;
}

inline void Speck::reverseRoundFunc(uint16_t& x, uint16_t& y, const uint16_t key)
{
	y ^= x;
	y = (y >> 2) | (y << 14);
	x ^= key;
	x -= y;
	x = (x << 7) | (x >> 9);
}

void Speck::encrypt(const uint16_t plaintext[2], const uint16_t key[4], 
	uint16_t ciphertext[2])
{
	ciphertext[0] = plaintext[0];
	ciphertext[1] = plaintext[1];
	uint16_t b = key[0];
	uint16_t a[3];
	a[0] = key[1];
	a[1] = key[2];
	a[2] = key[3];
	
	for (unsigned i = 0; i < 22; ++i) {
		roundFunc(ciphertext[1], ciphertext[0], b);
		roundFunc(a[i % 3], b, i);
	}
}

void Speck::decrypt(const uint16_t ciphertext[2], const uint16_t key[4],
	uint16_t plaintext[2])
{
	plaintext[0] = ciphertext[0];
	plaintext[1] = ciphertext[1];
	uint16_t b = key[0];
	uint16_t a[3];
	a[0] = key[1];
	a[1] = key[2];
	a[2] = key[3];
	uint16_t key_schedule[22]; 
	key_schedule[0] = b;
	for (unsigned i = 0; i < 21; ++i) {
		roundFunc(a[i % 3], b, i);
		key_schedule[i + 1] = b;
	}

	for (unsigned i = 22; i > 0; --i) {
		reverseRoundFunc(plaintext[1], plaintext[0], key_schedule[i-1]);
	}
}

// SPECK_64_128

inline void Speck::roundFunc(uint32_t& x, uint32_t& y, uint32_t key)
{
	x = (x >> 8) | (x << 24); // x = ROTR(x, 8)
	x += y;
	x ^= key;
	y = (y << 3) | (y >> 29); // y = ROTL(y, 3)
	y ^= x;
}

inline void Speck::reverseRoundFunc(uint32_t& x, uint32_t& y, uint32_t key)
{
	y ^= x;
	y = (y >> 3) | (y << 29);
	x ^= key;
	x -= y;
	x = (x << 8) | (x >> 24);
}

void Speck::encrypt(const uint32_t plaintext[2], const uint32_t key[4], 
	uint32_t ciphertext[2])
{
	ciphertext[0] = plaintext[0];
	ciphertext[1] = plaintext[1];
	uint32_t b = key[0];
	uint32_t a[3];
	a[0] = key[1];
	a[1] = key[2];
	a[2] = key[3];
	for (unsigned i = 0; i < 27; ++i) {
		roundFunc(ciphertext[1], ciphertext[0], b);
		roundFunc(a[i % 3], b, i);
	}
}

void Speck::decrypt(const uint32_t ciphertext[2], const uint32_t key[4],
	uint32_t plaintext[2])
{
	plaintext[0] = ciphertext[0];
	plaintext[1] = ciphertext[1];
	uint32_t key_schedule[27];
	uint32_t b = key[0];
	uint32_t a[3];
	a[0] = key[1];
	a[1] = key[2];
	a[2] = key[3];
	key_schedule[0] = b;
	for (unsigned i = 0; i < 26; ++i) {
		roundFunc(a[i % 3], b, i);
		key_schedule[i + 1] = b;
	}
	
	for (unsigned i = 27; i > 0; --i) {
		reverseRoundFunc(plaintext[1], plaintext[0], key_schedule[i - 1]);
	}
}
