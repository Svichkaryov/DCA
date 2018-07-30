#include "stdafx.h"
#include "Ciphers/Speck.h"


void test_speck_32_64()
{
	Speck s;
	uint16_t plaintext[2] = { 0x694c, 0x6574 };
	uint16_t key[4] = { 0x0100, 0x0908, 0x1110, 0x1918 };
	uint16_t ciphertext[2];
	uint16_t decrypted[2];

	s.encrypt(plaintext, key, ciphertext);

	s.decrypt(ciphertext, key, decrypted);
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

	s.encrypt(plaintext, key, ciphertext);
	s.decrypt(ciphertext, key, decrypted);

	printf("Plaintext:  0x%08x 0x%08x\n", plaintext[0], plaintext[1]);
	printf("Key:        0x%08x 0x%08x 0x%08x 0x%08x\n", key[0], key[1], key[2], key[3]);
	printf("Ciphertext: 0x%08x 0x%08x\n", ciphertext[0], ciphertext[1]);
	printf("Expected:   0x454e028b 0x8c6fa548\n");
	printf("Plaintext:  0x%08x 0x%08x\n", plaintext[0], plaintext[1]);
	printf("Key:        0x%08x 0x%08x 0x%08x 0x%08x\n", key[0], key[1], key[2], key[3]);
	printf("Ciphertext: 0x%08x 0x%08x\n", ciphertext[0], ciphertext[1]);
	printf("Expected:   0x454e028b 0x8c6fa548\n");
	printf("Decrypted:  0x%08x 0x%08x\n", decrypted[0], decrypted[1]);
}


int main()
{
	test_speck_64_128();
	
	system("pause");
    return 0;
}

