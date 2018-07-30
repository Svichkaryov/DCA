#pragma once


class Speck
{
public:
	Speck() = default;
	~Speck() = default;

	// SPECK_32_64
	
	inline void roundFunc(uint16_t& x, uint16_t& y, const uint16_t key);
	inline void reverseRoundFunc(uint16_t& x, uint16_t& y, const uint16_t key);

	void encrypt(const uint16_t plaintext[2], const uint16_t key[4], 
		uint16_t ciphertext[2]);
	void decrypt(const uint16_t ciphertext[2], const uint16_t key[4], 
		uint16_t plaintext[2]);

	// SPECK_64_128

	inline void roundFunc(uint32_t& x, uint32_t& y, const uint32_t key);
	inline void reverseRoundFunc(uint32_t& x, uint32_t& y, uint32_t key);

	void encrypt(const uint32_t plaintext[2], const uint32_t key[4],
		uint32_t ciphertext[2]);
	void decrypt(const uint32_t ciphertext[2], const uint32_t key[4],
		uint32_t plaintext[2]);

private:

};
