#pragma once
#include "CipherARX_32_64.h"


class Speck : public CipherARX_32_64
{
public:
	Speck() : CipherARX_32_64() {};
	Speck(OutputStateStategy oss, int nBitOuput) : CipherARX_32_64(oss, nBitOuput) {};
	~Speck() = default;

	// SPECK_32_64
	
	inline void round_func(uint16_t& x, uint16_t& y, const uint16_t key);
	inline void reverse_round_func(uint16_t& x, uint16_t& y, const uint16_t key);

	void encrypt_block(const uint16_t plaintext[2], const uint16_t key[4], 
		uint16_t ciphertext[2]) override;
	void decrypt_block(const uint16_t ciphertext[2], const uint16_t key[4], 
		uint16_t plaintext[2]) override;

	// SPECK_64_128

	inline void round_func(uint32_t& x, uint32_t& y, const uint32_t key);
	inline void reverse_round_func(uint32_t& x, uint32_t& y, uint32_t key);

	void encrypt_block(const uint32_t plaintext[2], const uint32_t key[4],
		uint32_t ciphertext[2]);
	void decrypt_block(const uint32_t ciphertext[2], const uint32_t key[4],
		uint32_t plaintext[2]);
private:

};

