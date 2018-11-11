#include "stdafx.h"
#include "CipherARX_32_64.h"


int CipherARX_32_64::get_bit(const uint16_t ciphertext[2])
{
	int outputBit = 0;
	int bitCount = 0;

	if (m_oss == OutputStateStategy::RAW_STATE)
	{
		if (m_nBitOutput < 16)
			outputBit = (ciphertext[0] >> m_nBitOutput) & 1;
		else
			outputBit = (ciphertext[1] >> (m_nBitOutput-16)) & 1;
	}
	else if (m_oss == OutputStateStategy::HW)
	{
		for (int y = 0; y < 16; ++y)
		{
			if (((ciphertext[0] >> y) & 1) == 1) { bitCount++; };
			if (((ciphertext[1] >> y) & 1) == 1) { bitCount++; };
		}
		outputBit = (bitCount >> m_nBitOutput) & 1;
	}
	else if (m_oss == OutputStateStategy::HW_BYTE)
	{
		int nBitOutput = m_nBitOutput & 0x000F;
		int nByteOutput = (m_nBitOutput & 0x00F0) >> 4;
		for (int i = 0; i < 8; ++i)
		{
			if (nByteOutput == 0x0 || nByteOutput == 0x1)
			{
				if (((ciphertext[0] >> (nByteOutput * 8 + i)) & 1) == 1) { bitCount++; };
			}
			else if (nByteOutput == 0x2 || nByteOutput == 0x3)
			{
				if (((ciphertext[1] >> (nByteOutput * 8 + i)) & 1) == 1) { bitCount++; };
			}
		}
		outputBit = (bitCount >> nBitOutput) & 1;
	}

	return outputBit;
}
