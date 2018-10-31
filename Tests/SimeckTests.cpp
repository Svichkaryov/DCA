#include "pch.h"
#include "../DCA/Ciphers/Simeck.h"


TEST(CipherTests, Simeck_32_64)
{
	Simeck s;
	uint16_t plaintext[2] = { 0x6877, 0x6565 };
	uint16_t key[4] = { 0x0100, 0x0908, 0x1110,  0x1918 };
	uint16_t ciphertext[2];
	uint16_t ciphertest_expected[2] = { 0x2c76, 0x770d };
	uint16_t decrypted[2];
	s.encrypt_block(plaintext, key, ciphertext);
	s.decrypt_block(ciphertext, key, decrypted);

	for (int i = 0; i < 2; ++i)
	{
		EXPECT_EQ(ciphertext[i], ciphertest_expected[i]);
		EXPECT_EQ(decrypted[i], plaintext[i]);
	}
}
