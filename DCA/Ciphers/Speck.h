#pragma once
#include "CipherARX_32_64.h"


class Speck : public CipherARX_32_64
{
public:
	Speck() : CipherARX_32_64() {};
	Speck(OutputStateStategy oss, int nBitOuput) : CipherARX_32_64(oss, nBitOuput) {};
	~Speck() = default;

	inline void round_func(uint16_t& x, uint16_t& y, const uint16_t key);
	inline void reverse_round_func(uint16_t& x, uint16_t& y, const uint16_t key);

	void encrypt_block(const uint16_t plaintext[2], const uint16_t key[4], 
		uint16_t ciphertext[2]) override;
	void decrypt_block(const uint16_t ciphertext[2], const uint16_t key[4], 
		uint16_t plaintext[2]) override;
	const char* cipher_info() override { return "Simeck_32_64_"; };

private:

};

