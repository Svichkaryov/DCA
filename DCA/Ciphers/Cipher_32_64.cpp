#include "stdafx.h"
#ifdef _MSC_VER
    #include <intrin.h>
#endif // _MSC_VER
#include "Cipher_32_64.h"


int Cipher_32_64::get_bit(const uint16_t ciphertext[2])
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
#ifdef _MSC_VER 
		bitCount += __popcnt16(ciphertext[0]);
		bitCount += __popcnt16(ciphertext[1]);
#elif 
		for (int y = 0; y < 16; ++y)
		{
			if (((ciphertext[0] >> y) & 1) == 1) { bitCount++; };
			if (((ciphertext[1] >> y) & 1) == 1) { bitCount++; };
		}
#endif // _MSC_VER 
		outputBit = (bitCount >> m_nBitOutput) & 1;
	}
	else if (m_oss == OutputStateStategy::HW_BYTE)
	{
		int nBitOutput = m_nBitOutput & 0x000F;
		int nByteOutput = (m_nBitOutput & 0x00F0) >> 4;

#ifdef _MSC_VER 
		if (nByteOutput == 0x0)
		{
			bitCount += __popcnt16(ciphertext[0] << 8);
		}
		else if (nByteOutput == 0x1)
		{
			bitCount += __popcnt16(ciphertext[0] >> 8);
		}
		else if (nByteOutput == 0x2)
		{
			bitCount += __popcnt16(ciphertext[1] << 8);
		}
		else if (nByteOutput == 0x3)
		{
			bitCount += __popcnt16(ciphertext[1] >> 8);
		}
#elif
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
#endif // _MSC_VER
		outputBit = (bitCount >> nBitOutput) & 1;
	}

	return outputBit;
}

const char* Cipher_32_64::get_outputStateStategy_name()
{
	switch (m_oss)
	{
	case OutputStateStategy::RAW_STATE:
		return "RAW_STATE";
	case OutputStateStategy::HW:
		return "HW";
	case OutputStateStategy::HW_BYTE:
		return "HW_BYTE";
	default:
		break;
	}
}
