#include "stdafx.h"
#include "DynamicCubeAttack.h"
#include "../../Ciphers/Simon.h"

void DynamicCubeAttack::upper_extension(uint16_t r_u, std::initializer_list<uint16_t> I)
{
	block_t key[KEY_WORDS] = { 0x12345, 0x67890};
	block_t key_u[KEY_WORDS] = {0};
	block_t S[2] = {0x0, 0x0};
	block_t S_dec[2];
	block_t S_enc[2];
	block_t plaintext[2];
	Simon s;

	for (auto &el : I)
	{
		if (el < 15)
			S[0] = S[0] | (1ull << el);
		else
			S[1] = S[1] | (1ull << el);
	}

	for (int t = 0; t < r_u; ++t)
	{
		s.partial_decrypt_block(S, key_u, S_dec, 1, r_u - t);
		
		for (int j = 0; j <= t; ++j)
		{
			s.partial_encrypt_block(S, key, S_dec, 1, r_u - t + j - 1);
			
			if (1)
			{
				// update key_u
			}
		}

		for(int i=0; i<2; ++i)
			S[i] = S_dec[i];
	}


}
