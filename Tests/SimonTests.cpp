#include "pch.h"
#include "../DCA/Ciphers/Simon.h"


TEST(CipherTests, Simon_32_64)
{
	Simon s;
	block_t plaintext[2];
	block_t ciphertext[2];
	block_t ciphertest_expected[2] = { 0xe9bb, 0xc69b };
	block_t decrypted[2];
	block_t key[4];

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
	
		for (int i = 0; i < 2; ++i)
		{
			EXPECT_EQ(ciphertext[i], ciphertest_expected[i]);
			EXPECT_EQ(decrypted[i], plaintext[i]);
		}
	}
}

TEST(CipherTests, Simon_48_72)
{
	Simon s;
	block_t plaintext[2];
	block_t ciphertext[2];
	block_t ciphertest_expected[2] = { 0xacf156, 0x6e06a5 };
	block_t decrypted[2];
	block_t key[4];

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

		for (int i = 0; i < 2; ++i)
		{
			EXPECT_EQ(ciphertext[i], ciphertest_expected[i]);
			EXPECT_EQ(decrypted[i], plaintext[i]);
		}
	}
}

TEST(CipherTests, Simon_48_96)
{
	Simon s;
	block_t plaintext[2];
	block_t ciphertext[2];
	block_t ciphertest_expected[2] = { 0x292cac, 0xdae5ac };
	block_t decrypted[2];
	block_t key[4];

	if (WORD_SIZE == 24 && KEY_WORDS == 3)
	{
		key[0] = 0x020100;
		key[1] = 0x0a0908;
		key[2] = 0x121110;
		plaintext[0] = 0x6e696c;
		plaintext[1] = 0x612067;

		s.encrypt_block(plaintext, key, ciphertext);
		s.decrypt_block(ciphertext, key, decrypted);

		for (int i = 0; i < 2; ++i)
		{
			EXPECT_EQ(ciphertext[i], ciphertest_expected[i]);
			EXPECT_EQ(decrypted[i], plaintext[i]);
		}
	}
}

TEST(CipherTests, Simon_64_96)
{
	Simon s;
	block_t plaintext[2];
	block_t ciphertext[2];
	block_t ciphertest_expected[2] = { 0x111a8fc8, 0x5ca2e27f };
	block_t decrypted[2];
	block_t key[4];

	if (WORD_SIZE == 32 && KEY_WORDS == 3)
	{
		key[0] = 0x03020100;
		key[1] = 0x0b0a0908;
		key[2] = 0x13121110;
		plaintext[0] = 0x6e696c63;
		plaintext[1] = 0x6f722067;

		s.encrypt_block(plaintext, key, ciphertext);
		s.decrypt_block(ciphertext, key, decrypted);

		for (int i = 0; i < 2; ++i)
		{
			EXPECT_EQ(ciphertext[i], ciphertest_expected[i]);
			EXPECT_EQ(decrypted[i], plaintext[i]);
		}
	}
}


TEST(CipherTests, Simon_64_128)
{
	Simon s;
	block_t plaintext[2];
	block_t ciphertext[2];
	block_t ciphertest_expected[2] = { 0xb9dfa07a, 0x44c8fc20 };
	block_t decrypted[2];
	block_t key[4];
	
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

		for (int i = 0; i < 2; ++i)
		{
			EXPECT_EQ(ciphertext[i], ciphertest_expected[i]);
			EXPECT_EQ(decrypted[i], plaintext[i]);
		}
	}
}

TEST(CipherTests, Simon_96_96)
{
	Simon s;
	block_t plaintext[2];
	block_t ciphertext[2];
	block_t ciphertest_expected[2] = { 0x69063d8ff082, 0x602807a462b4 };
	block_t decrypted[2];
	block_t key[4];

	if (WORD_SIZE == 48 && KEY_WORDS == 2)
	{
		key[0] = 0x050403020100;
		key[1] = 0x0d0c0b0a0908;
		plaintext[0] = 0x702065687420;
		plaintext[1] = 0x2072616c6c69;

		s.encrypt_block(plaintext, key, ciphertext);
		s.decrypt_block(ciphertext, key, decrypted);

		for (int i = 0; i < 2; ++i)
		{
			EXPECT_EQ(ciphertext[i], ciphertest_expected[i]);
			EXPECT_EQ(decrypted[i], plaintext[i]);
		}
	}
}

TEST(CipherTests, Simon_96_144)
{
	Simon s;
	block_t plaintext[2];
	block_t ciphertext[2];
	block_t ciphertest_expected[2] = { 0x3f59c5db1ae9, 0xecad1c6c451e };
	block_t decrypted[2];
	block_t key[4];

	if (WORD_SIZE == 48 && KEY_WORDS == 3)
	{
		key[0] = 0x050403020100;
		key[1] = 0x0d0c0b0a0908;
		key[2] = 0x151413121110;
		plaintext[0] = 0x73756420666f;
		plaintext[1] = 0x746168742074;

		s.encrypt_block(plaintext, key, ciphertext);
		s.decrypt_block(ciphertext, key, decrypted);

		for (int i = 0; i < 2; ++i)
		{
			EXPECT_EQ(ciphertext[i], ciphertest_expected[i]);
			EXPECT_EQ(decrypted[i], plaintext[i]);
		}
	}
}

TEST(CipherTests, Simon_128_128)
{
	Simon s;
	block_t plaintext[2];
	block_t ciphertext[2];
	block_t ciphertest_expected[2] = { 0x65aa832af84e0bbc, 0x49681b1e1e54fe3f };
	block_t decrypted[2];
	block_t key[4];

	if (WORD_SIZE == 64 && KEY_WORDS == 2)
	{
		key[0] = 0x0706050403020100;
		key[1] = 0x0f0e0d0c0b0a0908;
		plaintext[0] = 0x6c6c657661727420;
		plaintext[1] = 0x6373656420737265;

		s.encrypt_block(plaintext, key, ciphertext);
		s.decrypt_block(ciphertext, key, decrypted);

		for (int i = 0; i < 2; ++i)
		{
			EXPECT_EQ(ciphertext[i], ciphertest_expected[i]);
			EXPECT_EQ(decrypted[i], plaintext[i]);
		}
	}
}

TEST(CipherTests, Simon_128_192)
{
	Simon s;
	block_t plaintext[2];
	block_t ciphertext[2];
	block_t ciphertest_expected[2] = { 0x6c9c8d6e2597b85b, 0xc4ac61effcdc0d4f };
	block_t decrypted[2];
	block_t key[4];

	if (WORD_SIZE == 64 && KEY_WORDS == 3)
	{
		key[0] = 0x0706050403020100;
		key[1] = 0x0f0e0d0c0b0a0908;
		key[2] = 0x1716151413121110;
		plaintext[0] = 0x6568772065626972;
		plaintext[1] = 0x206572656874206e;

		s.encrypt_block(plaintext, key, ciphertext);
		s.decrypt_block(ciphertext, key, decrypted);

		for (int i = 0; i < 2; ++i)
		{
			EXPECT_EQ(ciphertext[i], ciphertest_expected[i]);
			EXPECT_EQ(decrypted[i], plaintext[i]);
		}
	}
}

TEST(CipherTests, Simon_128_256)
{
	Simon s;
	block_t plaintext[2];
	block_t ciphertext[2];
	block_t ciphertest_expected[2] = { 0x3bf72a87efe7b868, 0x8d2b5579afc8a3a0 };
	block_t decrypted[2];
	block_t key[4];

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

		for (int i = 0; i < 2; ++i)
		{
			EXPECT_EQ(ciphertext[i], ciphertest_expected[i]);
			EXPECT_EQ(decrypted[i], plaintext[i]);
		}
	}
}
