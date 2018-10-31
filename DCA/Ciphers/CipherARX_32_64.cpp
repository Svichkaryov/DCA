#include "stdafx.h"
#include "CipherARX_32_64.h"


int CipherARX_32_64::get_bit(const uint16_t ciphertext[2])
{
	int outputBit = 0;
	int bitCount = 0;

	if (m_oss == OutputStateStategy::HW)
	{
		for (int y = 0; y < 16; ++y)
		{
			if (((ciphertext[0] >> y) & 1) == 1) { bitCount++; };
			if (((ciphertext[1] >> y) & 1) == 1) { bitCount++; };
		}
		outputBit ^= (bitCount >> m_nBitOutput) & 1;
	}
	else if (m_oss == OutputStateStategy::RAW_STATE)
	{
		if (m_nBitOutput < 16)
			outputBit ^= (ciphertext[0] >> m_nBitOutput) & 1;
		else
			outputBit ^= (ciphertext[1] >> m_nBitOutput) & 1;
	}

	return outputBit;
}
