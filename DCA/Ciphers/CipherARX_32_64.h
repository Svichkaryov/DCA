#pragma once


enum class OutputStateStategy { HW, RAW_STATE };

class CipherARX_32_64
{
public:
	CipherARX_32_64() : m_oss(OutputStateStategy::HW), m_nBitOutput(1) {};
	CipherARX_32_64(OutputStateStategy oss, int nBitOuput) : m_oss(oss), m_nBitOutput(nBitOuput) {};

	virtual void encrypt_block(const uint16_t plaintext[2], const uint16_t key[4],
		uint16_t ciphertext[2]) = 0;
	virtual void decrypt_block(const uint16_t ciphertext[2], const uint16_t key[4],
		uint16_t plaintext[2]) = 0;

	int get_bit(uint16_t ciphertext[2]);

private:
	OutputStateStategy m_oss;
	int m_nBitOutput;
};

