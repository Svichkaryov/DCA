#include "stdafx.h"
#include "Simeck.h"
#ifdef _MSC_VER
    #include <intrin.h>
#endif // _MSC_VER

inline void Simeck::round_func(uint16_t& x, uint16_t& y, const uint16_t key)
{
	uint16_t tmp = x;
	x = y ^ ((x & (x << 5 | x >> 11)) ^ (x << 1 | x >> 15)) ^ key;
	y = tmp;
}

inline void Simeck::reverse_round_func(uint16_t& x, uint16_t& y, const uint16_t key)
{
	uint16_t tmp = y;
	y = x ^ ((y & (y << 5 | y >> 11)) ^ (y << 1 | y >> 15)) ^ key;
	x = tmp;
}

void Simeck::encrypt_block(const uint16_t plaintext[2], const uint16_t key[4], uint16_t ciphertext[2])
{
	ciphertext[0] = plaintext[0];
	ciphertext[1] = plaintext[1];
	uint16_t b = key[0];
	uint16_t a[3];
	a[0] = key[1];
	a[1] = key[2];
	a[2] = key[3];

	uint16_t constant = 0xFFFC;
	uint32_t sequence = 0x9A42BB1F;
	
	/*for (int i = 0; i < 32; ++i)
	{
		round_func(ciphertext[1], ciphertext[0], b);

		constant &= 0xFFFC;
		constant |= sequence & 1;
		sequence >>= 1;
		round_func(a[i % 3], b, constant);
	}*/

	// 1
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[0], b, constant);

	//2
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[1], b, constant);

	//3
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[2], b, constant);

	//4
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[0], b, constant);
	

/*
	//5
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[1], b, constant);


	//6
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[2], b, constant);

	//7
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[0], b, constant);

	//8
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[1], b, constant);

	//9
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[2], b, constant);

	//10
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[0], b, constant);

	//11
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[1], b, constant);

	//12
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[2], b, constant);

	//13
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[0], b, constant);

	//14
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[1], b, constant);

	//15
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[2], b, constant);

	//16
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[0], b, constant);

	//17
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[1], b, constant);

	//18
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[2], b, constant);

	//19
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[0], b, constant);

	//20
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[1], b, constant);

	//21
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[2], b, constant);

	//22
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[0], b, constant);

	//23
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[1], b, constant);

	//24
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[2], b, constant);
	
	//25
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[0], b, constant);
	
	//26
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[1], b, constant);
	
	//27
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[2], b, constant);
	
	//28
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[0], b, constant);
	
	//29
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[1], b, constant);
	
	//30
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[2], b, constant);
	
	//31
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[0], b, constant);
	
	//32
	round_func(ciphertext[1], ciphertext[0], b);
	constant &= 0xFFFC;
	constant |= sequence & 1;
	sequence >>= 1;
	round_func(a[1], b, constant);
*/
}


void Simeck::decrypt_block(const uint16_t ciphertext[2], const uint16_t key[4], uint16_t plaintext[2])
{
	plaintext[0] = ciphertext[0];
	plaintext[1] = ciphertext[1];
	uint16_t b = key[0];
	uint16_t a[3];
	a[0] = key[1];
	a[1] = key[2];
	a[2] = key[3];

	uint16_t constant = 0xFFFC;
	uint32_t sequence = 0x9A42BB1F;
	
	uint16_t key_schedule[32];
	key_schedule[0] = b;
	for (unsigned i = 0; i < 31; ++i)
	{
		constant &= 0xFFFC;
		constant |= sequence & 1;
		sequence >>= 1;
		round_func(a[i % 3], b, constant);	
		key_schedule[i + 1] = b;
	}

	for (unsigned i = 32; i > 0; --i) {
		reverse_round_func(plaintext[1], plaintext[0], key_schedule[i - 1]);
	}
}