#pragma once

#define WORD_SIZE 16
#define KEY_WORDS 4

#if (WORD_SIZE == 64)
	#define WORD_MASK (0xffffffffffffffffull)
#else
	#define WORD_MASK ((0x1ull << (WORD_SIZE&63)) - 1)
#endif

#define CONST_C ((0xffffffffffffffffull ^ 0x3ull) & WORD_MASK)

#if (WORD_SIZE == 16)
	#define ROUNDS (32)
	#define CONST_J (0)

#elif (WORD_SIZE == 24)
	#if (KEY_WORDS == 3)
		#define ROUNDS (36)
		#define CONST_J (0)
	#elif (KEY_WORDS == 4)
		#define ROUNDS (36)
		#define CONST_J (1)
	#endif

#elif (WORD_SIZE == 32)
	#if (KEY_WORDS == 3)
		#define ROUNDS (42)
		#define CONST_J (2)
	#elif (KEY_WORDS == 4)
		#define ROUNDS (44)
		#define CONST_J (3)
	#endif

#elif (WORD_SIZE == 48)
	#if (KEY_WORDS == 2)
		#define ROUNDS (52)
		#define CONST_J (2)
	#elif (KEY_WORDS == 3)
		#define ROUNDS (54)
		#define CONST_J (3)
	#endif

#elif (WORD_SIZE == 64)
	#if (KEY_WORDS == 2)
		#define ROUNDS (68)
		#define CONST_J (2)
	#elif (KEY_WORDS == 3)
		#define ROUNDS (69)
		#define CONST_J (3)
	#elif (KEY_WORDS == 4)
		#define ROUNDS (72)
		#define CONST_J (4)
	#endif

#endif

using block_t = uint64_t;

class Simon
{
public:
	Simon() = default;
	~Simon() = default;

	inline block_t S(block_t state, int distance);
	inline block_t F(block_t state);

	void encrypt_block(const block_t plaintext[2], const block_t key[KEY_WORDS], 
		block_t ciphertext[2]);
	void decrypt_block(const block_t ciphertext[2], const block_t key[KEY_WORDS],
		block_t plaintext[2]);
	
	void partial_encrypt_block(const block_t plaintext[2], const block_t key[KEY_WORDS], 
		block_t ciphertext[2], int rounds);
	void partial_decrypt_block(const block_t ciphertext[2], const block_t key[KEY_WORDS], 
		block_t plaintext[2], int rounds);

private:
	
	uint64_t z[5][62] = {
		{ 1,1,1,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0,
		  1,1,1,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0 },
		
		{ 1,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,1,0,
		  1,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,1,0 },
		
		{ 1,0,1,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,1,1,0,1,0,0,1,0,0,1,1,0,0,
		  0,1,0,1,0,0,0,0,1,0,0,0,1,1,1,1,1,1,0,0,1,0,1,1,0,1,1,0,0,1,1 },
		
		{ 1,1,0,1,1,0,1,1,1,0,1,0,1,1,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,0,0,
		  0,0,1,0,0,1,0,0,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,0,0,0,0,1,1,1,1 },
		
		{ 1,1,0,1,0,0,0,1,1,1,1,0,0,1,1,0,1,0,1,1,0,1,1,0,0,0,1,0,0,0,0,
		0,0,1,0,1,1,1,0,0,0,0,1,1,0,0,1,0,1,0,0,1,0,0,1,1,1,0,1,1,1,1 }
	};
};

