#pragma once


enum class OutputStateStategy { RAW_STATE, HW, HW_BYTE };

class CipherARX_32_64
{
public:
	CipherARX_32_64() : m_oss(OutputStateStategy::RAW_STATE), m_nBitOutput(1) {};
	CipherARX_32_64(OutputStateStategy oss, int nBitOuput) : m_oss(oss), m_nBitOutput(nBitOuput) {};
	virtual ~CipherARX_32_64() {};

	virtual void encrypt_block(const uint16_t plaintext[2], const uint16_t key[4],
		uint16_t ciphertext[2]) = 0;
	virtual void decrypt_block(const uint16_t ciphertext[2], const uint16_t key[4],
		uint16_t plaintext[2]) = 0;
	virtual const char* cipher_info() = 0;

	int get_bit(const uint16_t ciphertext[2]);
	int get_nBitOutput() { return m_nBitOutput; };
	const char* get_outputStateStategy_name();

private:
	OutputStateStategy m_oss;
	int m_nBitOutput;
};

