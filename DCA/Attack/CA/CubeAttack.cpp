#include "stdafx.h"
#include "CubeAttack.h"
#include "random"
#include "../../Ciphers/Speck.h"


void CubeAttack::preprocessing_phase()
{
}

void CubeAttack::online_phase()
{
}

void CubeAttack::user_mode()
{
}

bool CubeAttack::linear_test(std::initializer_list<int> maxterm)
{
	Speck s;
	uint16_t plaintext[2] = { 0x0, 0x0 };
	uint16_t ciphertext[2] = { 0x0, 0x0 };
	uint16_t x[4] = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t y[4] = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t xy[4] = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t nul[4] = { 0x0, 0x0, 0x0, 0x0 };

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 1 << 16);
	
	int bitCount = 0;
	int maxtermWeight = maxterm.size();
	uint32_t cardialDegree = 1U << maxtermWeight;
	uint32_t pt;

	int answer = 0;
	for (int i = 0; i < 100; ++i)
	{
		x[0] = dis(gen);
		x[1] = dis(gen);
		x[2] = dis(gen);
		x[3] = dis(gen);
		y[0] = dis(gen);
		y[1] = dis(gen);
		y[2] = dis(gen);
		y[3] = dis(gen);
		xy[0] = x[0] ^ y[0];
		xy[1] = x[1] ^ y[1];
		xy[2] = x[2] ^ y[2];
		xy[3] = x[3] ^ y[3];

		for (uint32_t k = 0; k < cardialDegree; ++k)
		{	
			for (int b = 0; b < maxtermWeight; ++b)
			{
				pt |= (k & (1U << b)) > 0 ?
					(1U << (*(maxterm.begin() + b))) : 0;
			}

			plaintext[0] = pt;
			plaintext[1] = pt >> 16;

			s.encrypt_block(plaintext, x, ciphertext);
			
			for (int s = 0; s < 16; ++s)
			{
				if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
				if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
			}

			answer ^= ((bitCount >> 1) & 1);
			bitCount = 0;
	
			s.encrypt_block(plaintext, y, ciphertext);
			
			for (int s = 0; s < 16; ++s)
			{
				if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
				if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
			}

			answer ^= ((bitCount >> 1) & 1);
			bitCount = 0;
			
			s.encrypt_block(plaintext, xy, ciphertext);
			
			for (int s = 0; s < 16; ++s)
			{
				if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
				if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
			}

			answer ^= ((bitCount >> 1) & 1);
			bitCount = 0;
			
			s.encrypt_block(plaintext, nul, ciphertext);

			for (int s = 0; s < 16; ++s)
			{
				if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
				if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
			}

			answer ^= ((bitCount >> 1) & 1);

			if (answer == 1) return false;
		}
	}

	return true;
}

std::vector<int> CubeAttack::compute_superpoly(std::initializer_list<int> maxterm)
{
	Speck s;
	uint16_t plaintext[2] = { 0x0, 0x0 };
	uint16_t ciphertext[2] = { 0x0, 0x0 };
	uint16_t key[4] = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t nul[4] = { 0x0, 0x0, 0x0, 0x0 };

	int constant = 0;
	int coeff = 0;
	int bitCount = 0;
	int maxtermWeight = maxterm.size();
	uint32_t cardialDegree = 1U << maxtermWeight;
	uint32_t pt = 0;
	uint64_t key_v = 0;

	std::vector<int> superpoly(65);

	for (int i = 0; i < cardialDegree; ++i)
	{
		for (int j = 0; j < maxtermWeight; ++j)
		{
			pt |= (i & (1U << j)) > 0 ?
				(1U << (*(maxterm.begin() + j))) : 0;
		}
		
		plaintext[0] = pt;
		plaintext[1] = pt >> 16;

		s.encrypt_block(plaintext, nul, ciphertext);

		for (int y = 0; y < 16; ++y)
		{
			if (((ciphertext[0] >> y) & 1) == 1) { bitCount++; };
			if (((ciphertext[1] >> y) & 1) == 1) { bitCount++; };
		}

		constant ^= ((bitCount >> 2) & 1);
		bitCount = 0;
		pt = 0;
	}
	superpoly[0] = constant;
	
	for (int k = 0; k < 64; ++k)
	{
		key_v = 1ULL << k;
		key[0] = key_v;
		key[1] = key_v >> 16;
		key[2] = key_v >> 32;
		key[3] = key_v >> 48;

		for (int i = 0; i < cardialDegree; ++i)
		{
			for (int j = 0; j < maxtermWeight; ++j)
			{
				pt |= (i & (1U << j)) > 0 ?
					(1U << (*(maxterm.begin() + j))) : 0;
			}

			plaintext[0] = pt;
			plaintext[1] = pt >> 16;

			s.encrypt_block(plaintext, key, ciphertext);

			for (int y = 0; y < 16; ++y)
			{
				if (((ciphertext[0] >> y) & 1) == 1) { bitCount++; };
				if (((ciphertext[1] >> y) & 1) == 1) { bitCount++; };
			}

			coeff ^= (bitCount >> 2) & 1;
			bitCount = 0;
			pt = 0;
		}
		superpoly[k + 1] = (constant ^ coeff);

		coeff = 0;
	}

	return superpoly;
}
