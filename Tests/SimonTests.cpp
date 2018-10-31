#include "pch.h"
#include "../DCA/Ciphers/Simon.h"


TEST(CipherTests, Simon_32_64)
{
	Simon s;
	uint16_t plaintext[2];
	uint16_t ciphertext[2];
	uint16_t ciphertest_expected[2] = { 0xe9bb, 0xc69b };
	uint16_t decrypted[2];
	uint16_t key[4];

	key[0] = 0x0100;
	key[1] = 0x0908;
	key[2] = 0x1110;
	key[3] = 0x1918;
	plaintext[0] = 0x6877;
	plaintext[1] = 0x6565;

	s.encrypt_block(plaintext, key, ciphertext);
	s.decrypt_block(ciphertext, key, decrypted);

	for (int i = 0; i < 2; ++i)
	{
		EXPECT_EQ(ciphertext[i], ciphertest_expected[i]);
		EXPECT_EQ(decrypted[i], plaintext[i]);
	}
}
