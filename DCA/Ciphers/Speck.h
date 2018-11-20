#pragma once
#include "Cipher_32_64.h"


class Speck : public Cipher_32_64
{
public:
	Speck() : Cipher_32_64(), n_round(22) {};
	Speck(int roundNum, OutputStateStategy oss, int nBitOuput) : Cipher_32_64(oss, nBitOuput), n_round(roundNum) {};
	~Speck() = default;

	inline void round_func(uint16_t& x, uint16_t& y, const uint16_t key);
	inline void reverse_round_func(uint16_t& x, uint16_t& y, const uint16_t key);

	void encrypt_block(const uint16_t plaintext[2], const uint16_t key[4], 
		uint16_t ciphertext[2]) override;
	void decrypt_block(const uint16_t ciphertext[2], const uint16_t key[4], 
		uint16_t plaintext[2]) override;
	const char* cipher_info() override { return "Speck_32_64"; };

private:
	int n_round;
};

