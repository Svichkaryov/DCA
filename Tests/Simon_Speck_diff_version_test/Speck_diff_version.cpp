#include "pch.h"
#include "../DCA/Ciphers/Speck.h


TEST(CipherTests, Speck_32_64)
{
	Speck s;
	uint16_t plaintext[2] = { 0x694c, 0x6574 };
	uint16_t key[4] = { 0x0100, 0x0908, 0x1110, 0x1918 };
	uint16_t ciphertext[2];
	uint16_t ciphertest_expected[2] = { 0x42f2, 0xa868 };
	uint16_t decrypted[2];

	s.encrypt_block(plaintext, key, ciphertext);
	s.decrypt_block(ciphertext, key, decrypted);

	for (int i = 0; i < 2; ++i)
	{
		EXPECT_EQ(ciphertext[i], ciphertest_expected[i]);
		EXPECT_EQ(decrypted[i], plaintext[i]);
	}
}

TEST(CipherTests, Speck_64_128)
{
	Speck s;
	uint32_t plaintext[2] = { 0x7475432dUL, 0x3b726574UL };
	uint32_t key[4] = { 0x03020100UL, 0x0b0a0908UL, 0x13121110UL, 0x1b1a1918UL };
	uint32_t ciphertext[2];
	uint32_t ciphertest_expected[2] = { 0x454e028b, 0x8c6fa548 };
	uint32_t decrypted[2];

	s.encrypt_block(plaintext, key, ciphertext);
	s.decrypt_block(ciphertext, key, decrypted);

	for (int i = 0; i < 2; ++i)
	{
		EXPECT_EQ(ciphertext[i], ciphertest_expected[i]);
		EXPECT_EQ(decrypted[i], plaintext[i]);
	}
}