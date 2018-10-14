#include "stdafx.h"
#include "Speck.h"
#include "windows.h"


// SPECK_32_64

Speck::Speck(OutputStateStategy oss, int nBitOutput)
{
	m_oss = oss;
	m_nBitOutput = nBitOutput;
}

inline void Speck::round_func(uint16_t& x, uint16_t& y, const uint16_t key)
{
	x = (x >> 0) | (x << 16);
	x += y;
	x ^= key;
	y = (y << 0) | (y >> 16);
	y ^= x;
}

inline void Speck::reverse_round_func(uint16_t& x, uint16_t& y, const uint16_t key)
{
	y ^= x;
	y = (y >> 2) | (y << 14);
	x ^= key;
	x -= y;
	x = (x << 7) | (x >> 9);
}

void Speck::encrypt_block(const uint16_t plaintext[2], const uint16_t key[4], 
	uint16_t ciphertext[2])
{
	ciphertext[0] = plaintext[0];
	ciphertext[1] = plaintext[1];
	uint16_t b = key[0];
	uint16_t a[3];
	a[0] = key[1];
	a[1] = key[2];
	a[2] = key[3];
	
	for (unsigned i = 0; i < 2; ++i) {
		round_func(ciphertext[1], ciphertext[0], b);
		round_func(a[i % 3], b, i);
	}
}

void Speck::decrypt_block(const uint16_t ciphertext[2], const uint16_t key[4],
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
		round_func(a[i % 3], b, i);
		key_schedule[i + 1] = b;
	}

	for (unsigned i = 22; i > 0; --i) {
		reverse_round_func(plaintext[1], plaintext[0], key_schedule[i-1]);
	}
}

// SPECK_64_128

inline void Speck::round_func(uint32_t& x, uint32_t& y, uint32_t key)
{
	x = (x >> 8) | (x << 24);
	x += y;
	x ^= key;
	y = (y << 3) | (y >> 29);
	y ^= x;
}

inline void Speck::reverse_round_func(uint32_t& x, uint32_t& y, uint32_t key)
{
	y ^= x;
	y = (y >> 3) | (y << 29);
	x ^= key;
	x -= y;
	x = (x << 8) | (x >> 24);
}

void Speck::encrypt_block(const uint32_t plaintext[2], const uint32_t key[4], 
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
		round_func(ciphertext[1], ciphertext[0], b);
		round_func(a[i % 3], b, i);
	}
}

void Speck::decrypt_block(const uint32_t ciphertext[2], const uint32_t key[4],
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
		round_func(a[i % 3], b, i);
		key_schedule[i + 1] = b;
	}
	
	for (unsigned i = 27; i > 0; --i) {
		reverse_round_func(plaintext[1], plaintext[0], key_schedule[i - 1]);
	}
}

int Speck::get_bit(uint16_t ciphertext[2])
{
	int output = 0;
	int bitCount = 0;

	if (m_oss == OutputStateStategy::HW)
	{
		for (int y = 0; y < 16; ++y)
		{
			if (((ciphertext[0] >> y) & 1) == 1) { bitCount++; };
			if (((ciphertext[1] >> y) & 1) == 1) { bitCount++; };
		}
		output ^= (bitCount >> m_nBitOutput) & 1;
	}
	else if (m_oss == OutputStateStategy::RAW_STATE)
	{
		if (m_nBitOutput < 16)
			output ^= (ciphertext[0] >> m_nBitOutput) & 1;
		else
			output ^= (ciphertext[1] >> m_nBitOutput) & 1;
	}
	
	return output;
}
