#include "stdafx.h"
#include "Ciphers/Speck.h"
#include "Ciphers/Simon.h"
#include "Utils/utils.h"

void test_speck_32_64()
{
	Speck s;
	uint16_t plaintext[2] = { 0x694c, 0x6574 };
	uint16_t key[4] = { 0x0100, 0x0908, 0x1110, 0x1918 };
	uint16_t ciphertext[2];
	uint16_t decrypted[2];

	s.encrypt_block(plaintext, key, ciphertext);
	s.decrypt_block(ciphertext, key, decrypted);

	printf("Plaintext:  0x%04x 0x%04x\n", plaintext[0], plaintext[1]);
	printf("Key:        0x%04x 0x%04x 0x%04x 0x%04x\n", key[0], key[1], key[2], key[3]);
	printf("Ciphertext: 0x%04x 0x%04x\n", ciphertext[0], ciphertext[1]);
	printf("Expected:   0x42f2 0xa868\n");
	printf("Plaintext:  0x%04x 0x%04x\n", decrypted[0], decrypted[1]);
}

void test_speck_64_128()
{
	Speck s;
	uint32_t plaintext[2] = { 0x7475432dUL, 0x3b726574UL };
	uint32_t key[4] = { 0x03020100UL, 0x0b0a0908UL, 0x13121110UL, 0x1b1a1918UL };
	uint32_t ciphertext[2];
	uint32_t decrypted[2];

	s.encrypt_block(plaintext, key, ciphertext);
	s.decrypt_block(ciphertext, key, decrypted);

	printf("Plaintext:  0x%08x 0x%08x\n", plaintext[0], plaintext[1]);
	printf("Key:        0x%08x 0x%08x 0x%08x 0x%08x\n", key[0], key[1], key[2], key[3]);
	printf("Ciphertext: 0x%08x 0x%08x\n", ciphertext[0], ciphertext[1]);
	printf("Expected:   0x454e028b 0x8c6fa548\n");
	printf("Decrypted:  0x%08x 0x%08x\n", decrypted[0], decrypted[1]);
}


int test_simon() {
	Simon s;
	block_t plaintext[2];
	block_t ciphertext[2];
	block_t decrypted[2];
	block_t key[4];

	// Simon32_64
	if (WORD_SIZE == 16 && KEY_WORDS == 4) 
	{
		key[0] = 0x0100;
		key[1] = 0x0908;
		key[2] = 0x1110;
		key[3] = 0x1918;
		plaintext[0] = 0x6877;
		plaintext[1] = 0x6565;
		
		s.encrypt_block(plaintext, key, ciphertext);
		s.decrypt_block(ciphertext, key, decrypted);

		printf("Plaintext:  0x%04x 0x%04x\n", plaintext[0], plaintext[1]);
		printf("Key:        0x%04x 0x%04x 0x%04x 0x%04x\n", key[0], key[1], key[2], key[3]);
		printf("Ciphertext: 0x%04x 0x%04x\n", ciphertext[0], ciphertext[1]);
		printf("Expected:   0xe9bb 0xc69b\n");
		printf("Decrypted:  0x%04x 0x%04x\n", decrypted[0], decrypted[1]);
	}

	// Simon48_72
	if (WORD_SIZE == 24 && KEY_WORDS == 3) 
	{
		key[0] = 0x020100;
		key[1] = 0x0a0908;
		key[2] = 0x121110;
		plaintext[0] = 0x6e696c;
		plaintext[1] = 0x612067;
		
		s.encrypt_block(plaintext, key, ciphertext);
		s.decrypt_block(ciphertext, key, decrypted);

		printf("Plaintext:  0x%06x 0x%06x\n", plaintext[0], plaintext[1]);
		printf("Key:        0x%06x 0x%06x 0x%06x\n", key[0], key[1], key[2]);
		printf("Ciphertext: 0x%06x 0x%06x\n", ciphertext[0], ciphertext[1]);
		printf("Expected:   0x292cac 0xdae5ac\n");
		printf("Decrypted:  0x%06x 0x%06x\n", decrypted[0], decrypted[1]);
	}

	// Simon48_96
	if (WORD_SIZE == 24 && KEY_WORDS == 4) 
	{
		key[0] = 0x020100;
		key[1] = 0x0a0908;
		key[2] = 0x121110;
		key[3] = 0x1a1918;
		plaintext[0] = 0x20646e;
		plaintext[1] = 0x726963;

		s.encrypt_block(plaintext, key, ciphertext);
		s.decrypt_block(ciphertext, key, decrypted);

		printf("Plaintext:  0x%06x 0x%06x\n", plaintext[0], plaintext[1]);
		printf("Key:        0x%06x 0x%06x 0x%06x 0x%06x\n", key[0], key[1], key[2], key[3]);
		printf("Ciphertext: 0x%06x 0x%06x\n", ciphertext[0], ciphertext[1]);
		printf("Expected:   0xacf156 0x6e06a5\n");
		printf("Decrypted:  0x%06x 0x%06x\n", decrypted[0], decrypted[1]);
	}

	// Simon64_96
	if (WORD_SIZE == 32 && KEY_WORDS == 3) 
	{
		key[0] = 0x03020100;
		key[1] = 0x0b0a0908;
		key[2] = 0x13121110;
		plaintext[0] = 0x6e696c63;
		plaintext[1] = 0x6f722067;

		s.encrypt_block(plaintext, key, ciphertext);
		s.decrypt_block(ciphertext, key, decrypted);

		printf("Plaintext:  0x%08x 0x%08x\n", plaintext[0], plaintext[1]);
		printf("Key:        0x%08x 0x%08x 0x%08x\n", key[0], key[1], key[2]);
		printf("Ciphertext: 0x%08x 0x%08x\n", ciphertext[0], ciphertext[1]);
		printf("Expected:   0x111a8fc8 0x5ca2e27f\n");
		printf("Decrypted:  0x%08x 0x%08x\n", decrypted[0], decrypted[1]);
	}

	// Simon64_128
	if (WORD_SIZE == 32 && KEY_WORDS == 4) 
	{
		key[0] = 0x03020100;
		key[1] = 0x0b0a0908;
		key[2] = 0x13121110;
		key[3] = 0x1b1a1918;
		plaintext[0] = 0x20646e75;
		plaintext[1] = 0x656b696c;

		s.encrypt_block(plaintext, key, ciphertext);
		s.decrypt_block(ciphertext, key, decrypted);

		printf("Plaintext:  0x%08x 0x%08x\n", plaintext[0], plaintext[1]);
		printf("Key:        0x%08x 0x%08x 0x%08x 0x%08x\n", key[0], key[1], key[2], key[3]);
		printf("Ciphertext: 0x%08x 0x%08x\n", ciphertext[0], ciphertext[1]);
		printf("Expected:   0xb9dfa07a 0x44c8fc20\n");
		printf("Decrypted:  0x%08x 0x%08x\n", decrypted[0], decrypted[1]);
	}

	// Simon96_96
	if (WORD_SIZE == 48 && KEY_WORDS == 2) 
	{
		key[0] = 0x050403020100;
		key[1] = 0x0d0c0b0a0908;
		plaintext[0] = 0x702065687420;
		plaintext[1] = 0x2072616c6c69;
		
		s.encrypt_block(plaintext, key, ciphertext);
		s.decrypt_block(ciphertext, key, decrypted);

		printf("Plaintext:  0x%012llx 0x%012llx\n", plaintext[0], plaintext[1]);
		printf("Key:        0x%012llx 0x%012llx\n", key[0], key[1]);
		printf("Ciphertext: 0x%012llx 0x%012llx\n", ciphertext[0], ciphertext[1]);
		printf("Expected:   0x69063d8ff082 0x602807a462b4\n");
		printf("Decrypted:  0x%012llx 0x%012llx\n", decrypted[0], decrypted[1]);
	}

	// Simon96_144
	if (WORD_SIZE == 48 && KEY_WORDS == 3) 
	{
		key[0] = 0x050403020100;
		key[1] = 0x0d0c0b0a0908;
		key[2] = 0x151413121110;
		plaintext[0] = 0x73756420666f;
		plaintext[1] = 0x746168742074;
	
		s.encrypt_block(plaintext, key, ciphertext);
		s.decrypt_block(ciphertext, key, decrypted);

		printf("Plaintext:  0x%012llx 0x%012llx\n", plaintext[0], plaintext[1]);
		printf("Key:        0x%012llx 0x%012llx 0x%012llx\n", key[0], key[1], key[2]);
		printf("Ciphertext: 0x%012llx 0x%012llx\n", ciphertext[0], ciphertext[1]);
		printf("Expected:   0x3f59c5db1ae9 0xecad1c6c451e\n");
		printf("Decrypted:  0x%012llx 0x%012llx\n", decrypted[0], decrypted[1]);
	}

	// Simon128_128
	if (WORD_SIZE == 64 && KEY_WORDS == 2) 
	{
		key[0] = 0x0706050403020100;
		key[1] = 0x0f0e0d0c0b0a0908;
		plaintext[0] = 0x6c6c657661727420;
		plaintext[1] = 0x6373656420737265;
		
		s.encrypt_block(plaintext, key, ciphertext);
		s.decrypt_block(ciphertext, key, decrypted);

		printf("Plaintext:  0x%016llx 0x%016llx\n", plaintext[0], plaintext[1]);
		printf("Key:        0x%016llx 0x%016llx\n", key[0], key[1]);
		printf("Ciphertext: 0x%016llx 0x%016llx\n", ciphertext[0], ciphertext[1]);
		printf("Expected:   0x65aa832af84e0bbc 0x49681b1e1e54fe3f\n");
		printf("Decrypted:  0x%016llx 0x%016llx\n", decrypted[0], decrypted[1]);
	}

	// Simon128_192
	if (WORD_SIZE == 64 && KEY_WORDS == 3) 
	{
		key[0] = 0x0706050403020100;
		key[1] = 0x0f0e0d0c0b0a0908;
		key[2] = 0x1716151413121110;
		plaintext[0] = 0x6568772065626972;
		plaintext[1] = 0x206572656874206e;
		
		s.encrypt_block(plaintext, key, ciphertext);
		s.decrypt_block(ciphertext, key, decrypted);

		printf("Plaintext:  0x%016llx 0x%016llx\n", plaintext[0], plaintext[1]);
		printf("Key:        0x%016llx 0x%016llx 0x%016llx\n", key[0], key[1], key[2]);
		printf("Ciphertext: 0x%016llx 0x%016llx\n", ciphertext[0], ciphertext[1]);
		printf("Expected:   0x6c9c8d6e2597b85b 0xc4ac61effcdc0d4f\n");
		printf("Decrypted:  0x%016llx 0x%016llx\n", decrypted[0], decrypted[1]);

	}

	// Simon128_256
	if (WORD_SIZE == 64 && KEY_WORDS == 4) 
	{
		key[0] = 0x0706050403020100;
		key[1] = 0x0f0e0d0c0b0a0908;
		key[2] = 0x1716151413121110;
		key[3] = 0x1f1e1d1c1b1a1918;
		plaintext[0] = 0x6d69732061207369;
		plaintext[1] = 0x74206e69206d6f6f;
		
		s.encrypt_block(plaintext, key, ciphertext);
		s.decrypt_block(ciphertext, key, decrypted);

		printf("Plaintext:  0x%016llx 0x%016llx\n", plaintext[0], plaintext[1]);
		printf("Key:        0x%016llx 0x%016llx 0x%016llx 0x%016llx\n", key[0], key[1], key[2], key[3]);
		printf("Ciphertext: 0x%016llx 0x%016llx\n", ciphertext[0], ciphertext[1]);
		printf("Expected:   0x3bf72a87efe7b868 0x8d2b5579afc8a3a0\n");
		printf("Decrypted:  0x%016llx 0x%016llx\n", decrypted[0], decrypted[1]);

	}

	return 0;
}

int f(int a)
{
	return 1;
}

int main()
{
	//test_speck_64_128();
	//test_simon();
	utils::executeAndGetElapsedTime(f, "f", 1);
	system("pause");
    return 0;
}

