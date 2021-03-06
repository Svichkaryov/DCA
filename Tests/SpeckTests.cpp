#include "pch.h"
#include "../DCA/Ciphers/Speck.h"


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