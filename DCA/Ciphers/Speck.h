#pragma once

enum class OutputStateStategy { HW, RAW_STATE };

class Speck
{
public:
	Speck() = default;
	Speck(OutputStateStategy oss, int nBitOuput);
	~Speck() = default;

	// SPECK_32_64
	
	inline void round_func(uint16_t& x, uint16_t& y, const uint16_t key);
	inline void reverse_round_func(uint16_t& x, uint16_t& y, const uint16_t key);

	void encrypt_block(const uint16_t plaintext[2], const uint16_t key[4], 
		uint16_t ciphertext[2]);
	void decrypt_block(const uint16_t ciphertext[2], const uint16_t key[4], 
		uint16_t plaintext[2]);

	// SPECK_64_128

	inline void round_func(uint32_t& x, uint32_t& y, const uint32_t key);
	inline void reverse_round_func(uint32_t& x, uint32_t& y, uint32_t key);

	void encrypt_block(const uint32_t plaintext[2], const uint32_t key[4],
		uint32_t ciphertext[2]);
	void decrypt_block(const uint32_t ciphertext[2], const uint32_t key[4],
		uint32_t plaintext[2]);

	int get_bit(uint16_t ciphertext[2]);

private:
	OutputStateStategy m_oss;
	int m_nBitOutput;

};

