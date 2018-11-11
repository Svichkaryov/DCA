#pragma once
#include "CipherARX_32_64.h"


class Simeck : public CipherARX_32_64
{
public:
	Simeck() : CipherARX_32_64() {};
	Simeck(OutputStateStategy oss, int nBitOuput) : CipherARX_32_64(oss, nBitOuput) {};
	~Simeck() = default;

	inline void round_func(uint16_t& x, uint16_t& y, const uint16_t key);
	inline void reverse_round_func(uint16_t& x, uint16_t& y, const uint16_t key);

	void encrypt_block(const uint16_t plaintext[2], const uint16_t key[4],
		uint16_t ciphertext[2]) override;
	void decrypt_block(const uint16_t ciphertext[2], const uint16_t key[4],
		uint16_t plaintext[2]) override;
	const char* cipher_info() override { return "Simeck32_64"; };

private:

};