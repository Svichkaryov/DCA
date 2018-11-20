#pragma once
#include "Cipher_32_64.h"

#define WORD_MASK ((0x1ull << (16&63)) - 1)
#define CONST_C ((0xffffffffffffffffull ^ 0x3ull) & WORD_MASK)
#define CONST_J (0)


class Simon : public Cipher_32_64
{
public:
	Simon() : Cipher_32_64(), n_round(32) {};
	Simon(int roundNum, OutputStateStategy oss, int nBitOuput) : Cipher_32_64(oss, nBitOuput), n_round(roundNum) {};
	~Simon() = default;

	inline uint16_t S(uint16_t state, int distance);
	inline uint16_t f(uint16_t state);

	void encrypt_block(const uint16_t plaintext[2], const uint16_t key[4],
		uint16_t ciphertext[2]);
	void decrypt_block(const uint16_t ciphertext[2], const uint16_t key[4],
		uint16_t plaintext[2]);
	
	void partial_encrypt_block(const uint16_t plaintext[2], const uint16_t key[4],
		uint16_t ciphertext[2], int rounds, int startInRound = 0);
	void partial_decrypt_block(const uint16_t ciphertext[2], const uint16_t key[4],
		uint16_t plaintext[2], int rounds, int startInRound = 32);
	const char* cipher_info() override { return "Simon_32_64"; };

private:
	int n_round;

	uint64_t z[5][62] = {
		{ 1,1,1,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0,
		  1,1,1,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0 },
		
		{ 1,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,1,0,
		  1,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,1,0 },
		
		{ 1,0,1,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,1,1,0,1,0,0,1,0,0,1,1,0,0,
		  0,1,0,1,0,0,0,0,1,0,0,0,1,1,1,1,1,1,0,0,1,0,1,1,0,1,1,0,0,1,1 },
		
		{ 1,1,0,1,1,0,1,1,1,0,1,0,1,1,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,0,0,
		  0,0,1,0,0,1,0,0,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,0,0,0,0,1,1,1,1 },
		
		{ 1,1,0,1,0,0,0,1,1,1,1,0,0,1,1,0,1,0,1,1,0,1,1,0,0,0,1,0,0,0,0,
		0,0,1,0,1,1,1,0,0,0,0,1,1,0,0,1,0,1,0,0,1,0,0,1,1,1,0,1,1,1,1 }
	};
};

