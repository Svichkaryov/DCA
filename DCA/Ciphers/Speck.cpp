#include "stdafx.h"
#include "Speck.h"
#include "windows.h"


inline void Speck::round_func(uint16_t& x, uint16_t& y, const uint16_t key)
{
	x = (x >> 7) | (x << 9);
	x += y;
	x ^= key;
	y = (y << 2) | (y >> 14);
	y ^= x;
}

inline void Speck::reverse_round_func(uint16_t& x, uint16_t& y, const uint16_t key)
{
	y ^= x;
	y = (y >> 2) | (y << 14);
	x ^= key;
	x -= y;
	x = (x << 7) | (x >> 9);
}

void Speck::encrypt_block(const uint16_t plaintext[2], const uint16_t key[4], 
	uint16_t ciphertext[2])
{
	ciphertext[0] = plaintext[0];
	ciphertext[1] = plaintext[1];
	uint16_t b = key[0];
	uint16_t a[3];
	a[0] = key[1];
	a[1] = key[2];
	a[2] = key[3];
	
	for (unsigned i = 0; i < n_round; ++i) 
	{
		round_func(ciphertext[1], ciphertext[0], b);
		round_func(a[i % 3], b, i);
	}

/*
	//1
	round_func(ciphertext[1], ciphertext[0], b);
	round_func(a[0], b, 0);
	
	//2
	round_func(ciphertext[1], ciphertext[0], b);
	round_func(a[1], b, 1);

	//3
	round_func(ciphertext[1], ciphertext[0], b);
	round_func(a[2], b, 2);

	//4
	round_func(ciphertext[1], ciphertext[0], b);
	round_func(a[0], b, 3);

	//5
	round_func(ciphertext[1], ciphertext[0], b);
	round_func(a[1], b, 4);

	//6
	round_func(ciphertext[1], ciphertext[0], b);
	round_func(a[2], b, 5);

	//7
	round_func(ciphertext[1], ciphertext[0], b);
	round_func(a[0], b, 6);

	//8
	round_func(ciphertext[1], ciphertext[0], b);
	round_func(a[1], b, 7);

	//9
	round_func(ciphertext[1], ciphertext[0], b);
	round_func(a[2], b, 8);

	//10
	round_func(ciphertext[1], ciphertext[0], b);
	round_func(a[0], b, 9);

	//11
	round_func(ciphertext[1], ciphertext[0], b);
	round_func(a[1], b, 10);

	//12
	round_func(ciphertext[1], ciphertext[0], b);
	round_func(a[2], b, 11);

	//13
	round_func(ciphertext[1], ciphertext[0], b);
	round_func(a[0], b, 12);

	//14
	round_func(ciphertext[1], ciphertext[0], b);
	round_func(a[1], b, 13);

	//15
	round_func(ciphertext[1], ciphertext[0], b);
	round_func(a[2], b, 14);

	//16
	round_func(ciphertext[1], ciphertext[0], b);
	round_func(a[0], b, 15);

	//17
	round_func(ciphertext[1], ciphertext[0], b);
	round_func(a[1], b, 16);

	//18
	round_func(ciphertext[1], ciphertext[0], b);
	round_func(a[2], b, 17);

	//19
	round_func(ciphertext[1], ciphertext[0], b);
	round_func(a[0], b, 18);

	//20
	round_func(ciphertext[1], ciphertext[0], b);
	round_func(a[1], b, 19);

	//21
	round_func(ciphertext[1], ciphertext[0], b);
	round_func(a[2], b, 20);

	//22
	round_func(ciphertext[1], ciphertext[0], b);
	round_func(a[0], b, 21);
*/
}

void Speck::decrypt_block(const uint16_t ciphertext[2], const uint16_t key[4],
	uint16_t plaintext[2])
{
	plaintext[0] = ciphertext[0];
	plaintext[1] = ciphertext[1];
	uint16_t b = key[0];
	uint16_t a[3];
	a[0] = key[1];
	a[1] = key[2];
	a[2] = key[3];
	uint16_t key_schedule[22]; 
	key_schedule[0] = b;
	for (unsigned i = 0; i < 21; ++i) {
		round_func(a[i % 3], b, i);
		key_schedule[i + 1] = b;
	}

	for (unsigned i = 22; i > 0; --i) {
		reverse_round_func(plaintext[1], plaintext[0], key_schedule[i-1]);
	}
}