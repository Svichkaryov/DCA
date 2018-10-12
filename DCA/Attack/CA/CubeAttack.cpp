#include "stdafx.h"
#include "random"
#include "sstream"
#include "CubeAttack.h"
#include "../../Ciphers/Speck.h"
#include "CubeFormer.h"


void CubeAttack::preprocessing_phase()
{
	uint32_t startCube = cubeFormer.get_start_cube(5);
	uint32_t nextCube = startCube;

	uint64_t linear_superpoly[2];
	std::vector<std::vector<int>> quadratic_superpoly;
	int count = 0;
	int cubeCount = cubeFormer.get_end_flag(5);

	while (count != cubeCount)
	{
		if (linear_test(nextCube))
		{
			std::vector<int> cubeIndexes = {};

			for (int i = 0; i < 32; ++i)
			{
				if (((nextCube >> i) & 1) == 1)
					cubeIndexes.push_back(i);
			}
			std::cout << "Cube in testing: { ";
			for (auto& el : cubeIndexes)
			{
				std::cout << el << " ";
			}
			std::cout << "}\n";

			compute_linear_superpoly(nextCube, linear_superpoly);
			print_linear_superpoly(linear_superpoly);
		}

		if (quadratic_test(nextCube))
		{
			std::vector<int> cubeIndexes = {};

			for (int i = 0; i < 32; ++i)
			{
				if (((nextCube >> i) & 1) == 1)
					cubeIndexes.push_back(i);
			}
			std::cout << "Cube in testing: { ";
			for (auto& el : cubeIndexes)
			{
				std::cout << el << " ";
			}
			std::cout << "}\n";

			compute_quadratic_superpoly(nextCube,
				find_secret_variables(nextCube), quadratic_superpoly);
			print_quadratic_superpoly(quadratic_superpoly);
		}
		count++;
		nextCube = cubeFormer.next_cube(nextCube);
	}
	std::cout << "Count = " << count << std::endl;
}

void CubeAttack::online_phase()
{

}

void CubeAttack::user_mode()
{
	char action;
	uint32_t maxterm;
	uint64_t linear_superpoly[2];
	std::vector<std::vector<int>> quadratic_superpoly;
	do
	{
		do 
		{
			printf("Input maxterm: ");
			if (!std::cin) 
			{
				printf("Invalid input\n");
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		} while (!(std::cin >> maxterm));

		if (linear_test(maxterm))
		{
			compute_linear_superpoly(maxterm, linear_superpoly);
			print_linear_superpoly(linear_superpoly);
		}

		if (quadratic_test(maxterm))
		{
			auto a = find_secret_variables(maxterm);
			compute_quadratic_superpoly(maxterm, 
				find_secret_variables(maxterm), quadratic_superpoly);
			print_quadratic_superpoly(quadratic_superpoly);
		}

		printf("action( Next(...) / Exit(e) ): " );
		std::cin >> action;

	} while (action != 'e');
}

bool CubeAttack::linear_test(uint32_t maxterm)
{
	Speck s;
	uint16_t plaintext[2]  = { 0x0, 0x0 };
	uint16_t ciphertext[2] = { 0x0, 0x0 };
	uint16_t x[4]          = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t y[4]          = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t xy[4]         = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t nul[4]        = { 0x0, 0x0, 0x0, 0x0 };

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 15);
	
	int bitCount = 0;
	int maxtermCount = 0;
	std::vector<int> cubeIndexes = {};

	for (int i = 0; i < 32; ++i)
	{
		if (((maxterm >> i) & 1) == 1)
		{
			maxtermCount++;
			cubeIndexes.push_back(i);
		}
	}

	uint32_t cardialDegree = 1U << maxtermCount;
	uint32_t pt;

	int answer = 0;
	for (int i = 0; i < 100; ++i)
	{
		x[0] = dis(gen);
		x[1] = dis(gen);
		x[2] = dis(gen);
		x[3] = dis(gen);

		y[0] = dis(gen);
		y[1] = dis(gen);
		y[2] = dis(gen);
		y[3] = dis(gen);

		xy[0] = x[0] ^ y[0];
		xy[1] = x[1] ^ y[1];
		xy[2] = x[2] ^ y[2];
		xy[3] = x[3] ^ y[3];

		for (uint32_t k = 0; k < cardialDegree; ++k)
		{
			for (int b = 0; b < maxtermCount; ++b)
			{
				if ((k & (1U << b)) > 0)
					pt |= (1U << cubeIndexes[b]);
				else
					pt &= ~(1U << cubeIndexes[b]);
			}
			plaintext[0] = pt;
			plaintext[1] = pt >> 16;

			s.encrypt_block(plaintext, x, ciphertext);
			for (int s = 0; s < 16; ++s)
			{
				if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
				if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
			}
			answer ^= (bitCount >> 1) & 1;
			bitCount = 0;

			s.encrypt_block(plaintext, y, ciphertext);
			for (int s = 0; s < 16; ++s)
			{
				if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
				if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
			}
			answer ^= ((bitCount >> 1) & 1);
			bitCount = 0;

			s.encrypt_block(plaintext, xy, ciphertext);
			for (int s = 0; s < 16; ++s)
			{
				if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
				if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
			}
			answer ^= ((bitCount >> 1) & 1);
			bitCount = 0;

			s.encrypt_block(plaintext, nul, ciphertext);
			for (int s = 0; s < 16; ++s)
			{
				if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
				if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
			}
			answer ^= ((bitCount >> 1) & 1);
			bitCount = 0;

			if (answer == 1) return false;
		}
		answer = 0;
	}

	return true;
}
void CubeAttack::compute_linear_superpoly(uint32_t maxterm, uint64_t superpoly[2])
{
	Speck s;
	uint16_t plaintext[2] = { 0x0, 0x0 };
	uint16_t ciphertext[2] = { 0x0, 0x0 };
	uint16_t key[4] = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t nul[4] = { 0x0, 0x0, 0x0, 0x0 };

	int constant = 0;
	int coeff = 0;
	int bitCount = 0;

	int maxtermCount = 0;
	std::vector<int> cubeIndexes = {};
	for (int i = 0; i < 32; ++i)
	{
		if (((maxterm >> i) & 1) == 1)
		{
			maxtermCount++;
			cubeIndexes.push_back(i);
		}
	}

	uint32_t cardialDegree = 1U << maxtermCount;
	uint32_t pt = 0;
	uint64_t keyInt = 0;
	superpoly[0] = 0;
	superpoly[1] = 0;

	for (uint32_t i = 0; i < cardialDegree; ++i)
	{
		for (int j = 0; j < maxtermCount; ++j)
		{
			if ((i & (1U << j)) > 0)
				pt |= (1U << cubeIndexes[j]);
			else
				pt &= ~(1U << cubeIndexes[j]);
		}
		plaintext[0] = pt;
		plaintext[1] = pt >> 16;

		s.encrypt_block(plaintext, nul, ciphertext);
		for (int y = 0; y < 16; ++y)
		{
			if (((ciphertext[0] >> y) & 1) == 1) { bitCount++; };
			if (((ciphertext[1] >> y) & 1) == 1) { bitCount++; };
		}
		constant ^= (bitCount >> 1) & 1;
		bitCount = 0;
	}
	superpoly[1] = constant;

	for (int k = 0; k < 64; ++k)
	{
		keyInt = 1ULL << k;
		key[0] = keyInt;
		key[1] = keyInt >> 16;
		key[2] = keyInt >> 32;
		key[3] = keyInt >> 48;

		for (uint32_t i = 0; i < cardialDegree; ++i)
		{
			for (int j = 0; j < maxtermCount; ++j)
			{
				if ((i & (1U << j)) > 0)
					pt |= (1U << cubeIndexes[j]);
				else
					pt &= ~(1U << cubeIndexes[j]);
			}
			plaintext[0] = pt;
			plaintext[1] = pt >> 16;

			s.encrypt_block(plaintext, key, ciphertext);
			for (int y = 0; y < 16; ++y)
			{
				if (((ciphertext[0] >> y) & 1) == 1) { bitCount++; };
				if (((ciphertext[1] >> y) & 1) == 1) { bitCount++; };
			}
			coeff ^= (bitCount >> 1) & 1;
			bitCount = 0;
		}
		if ((constant ^ coeff) == 1)
			superpoly[0] |= 1ULL << k;

		coeff = 0;
	}
}

void CubeAttack::print_linear_superpoly(const uint64_t superpoly[2])
{
	std::ostringstream ls;
	ls << superpoly[1];
	for (int i = 0; i < 64; ++i)
	{
		if (((superpoly[0] >> i) & 1) == 1)
			ls << "+x" << i;
	}
	if(superpoly[0]>0)
		std::cout << ls.str() << std::endl;
}

bool CubeAttack::quadratic_test(uint32_t maxterm)
{
	Speck s;
	uint16_t plaintext[2]  = { 0x0, 0x0 };
	uint16_t ciphertext[2] = { 0x0, 0x0 };
	uint16_t x[4]          = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t y[4]          = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t z[4]          = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t xy[4]         = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t xz[4]         = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t yz[4]         = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t xyz[4]        = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t nul[4]        = { 0x0, 0x0, 0x0, 0x0 };

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 15);

	int bitCount = 0;
	int maxtermCount = 0;
	std::vector<int> cubeIndexes = {};

	for (int i = 0; i < 32; ++i)
	{
		if (((maxterm >> i) & 1) == 1)
		{
			maxtermCount++;
			cubeIndexes.push_back(i);
		}
	}

	uint32_t cardialDegree = 1U << maxtermCount;
	uint32_t pt;

	int answer = 0;
	for (int i = 0; i < 100; ++i)
	{
		x[0] = dis(gen);
		x[1] = dis(gen);
		x[2] = dis(gen);
		x[3] = dis(gen);

		y[0] = dis(gen);
		y[1] = dis(gen);
		y[2] = dis(gen);
		y[3] = dis(gen);
		
		z[0] = dis(gen);
		z[1] = dis(gen);
		z[2] = dis(gen);
		z[3] = dis(gen);

		xy[0] = x[0] ^ y[0];
		xy[1] = x[1] ^ y[1];
		xy[2] = x[2] ^ y[2];
		xy[3] = x[3] ^ y[3];

		xz[0] = x[0] ^ z[0];
		xz[1] = x[1] ^ z[1];
		xz[2] = x[2] ^ z[2];
		xz[3] = x[3] ^ z[3];

		yz[0] = y[0] ^ z[0];
		yz[1] = y[1] ^ z[1];
		yz[2] = y[2] ^ z[2];
		yz[3] = y[3] ^ z[3];

		xyz[0] = x[0] ^ y[0] ^ z[0];
		xyz[1] = x[1] ^ y[1] ^ z[1];
		xyz[2] = x[2] ^ y[2] ^ z[2];
		xyz[3] = x[3] ^ y[3] ^ z[3];

		for (uint32_t k = 0; k < cardialDegree; ++k)
		{
			for (int b = 0; b < maxtermCount; ++b)
			{
				if ((k & (1U << b)) > 0)
					pt |= (1U << cubeIndexes[b]);
				else
					pt &= ~(1U << cubeIndexes[b]);
			}
			plaintext[0] = pt;
			plaintext[1] = pt >> 16;

			s.encrypt_block(plaintext, x, ciphertext);
			for (int s = 0; s < 16; ++s)
			{
				if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
				if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
			}
			answer ^= (bitCount >> 1) & 1;
			bitCount = 0;

			s.encrypt_block(plaintext, y, ciphertext);
			for (int s = 0; s < 16; ++s)
			{
				if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
				if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
			}
			answer ^= ((bitCount >> 1) & 1);
			bitCount = 0;

			s.encrypt_block(plaintext, z, ciphertext);
			for (int s = 0; s < 16; ++s)
			{
				if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
				if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
			}
			answer ^= ((bitCount >> 1) & 1);
			bitCount = 0;

			s.encrypt_block(plaintext, xy, ciphertext);
			for (int s = 0; s < 16; ++s)
			{
				if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
				if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
			}
			answer ^= ((bitCount >> 1) & 1);
			bitCount = 0;

			s.encrypt_block(plaintext, xz, ciphertext);
			for (int s = 0; s < 16; ++s)
			{
				if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
				if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
			}
			answer ^= ((bitCount >> 1) & 1);
			bitCount = 0;

			s.encrypt_block(plaintext, yz, ciphertext);
			for (int s = 0; s < 16; ++s)
			{
				if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
				if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
			}
			answer ^= ((bitCount >> 1) & 1);
			bitCount = 0;

			s.encrypt_block(plaintext, xyz, ciphertext);
			for (int s = 0; s < 16; ++s)
			{
				if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
				if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
			}
			answer ^= ((bitCount >> 1) & 1);
			bitCount = 0;

			s.encrypt_block(plaintext, nul, ciphertext);
			for (int s = 0; s < 16; ++s)
			{
				if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
				if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
			}
			answer ^= ((bitCount >> 1) & 1);
			bitCount = 0;

			if (answer == 1) return false;
		}
		answer = 0;
	}

	return true;
}

uint64_t CubeAttack::find_secret_variables(uint32_t maxterm)
{
	int maxtermCount = 0;
	std::vector<int> cubeIndexes = {};
	for (int i = 0; i < 32; ++i)
	{
		if (((maxterm >> i) & 1) == 1)
		{
			maxtermCount++;
			cubeIndexes.push_back(i);
		}
	}
	uint32_t cardialDegree = 1U << maxtermCount;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 15);

	int output = 0;
	int bitCount = 0;
	uint64_t secretVariablesIndexes = 0;

	Speck s;
	uint16_t plaintext[2]  = { 0x0, 0x0 };
	uint16_t ciphertext[2] = { 0x0, 0x0 };
	uint16_t key[4]        = { 0x0, 0x0, 0x0, 0x0 };
	uint32_t pt            = 0x0;
	uint64_t keyInt        = 0x0;

	for (size_t i = 0; i < 64; ++i)
	{
		for (int k = 0; k < 300; ++k)
		{
			key[0] = dis(gen);
			key[1] = dis(gen);
			key[2] = dis(gen);
			key[3] = dis(gen);

			for (uint32_t j = 0; j < cardialDegree; ++j)
			{
				for (int b = 0; b < maxtermCount; ++b)
				{
					if ((j & (1U << b)) > 0)
						pt |= (1U << cubeIndexes[b]);
					else
						pt &= ~(1U << cubeIndexes[b]);
				}
				plaintext[0] = pt;
				plaintext[1] = pt >> 16;

				keyInt  = 1ULL << i;
				key[0] |= keyInt;
				key[1] |= keyInt >> 16;
				key[2] |= keyInt >> 32;
				key[3] |= keyInt >> 48;
				keyInt  = 0;

				s.encrypt_block(plaintext, key, ciphertext);
				for (int s = 0; s < 16; ++s)
				{
					if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
					if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
				}
				output ^= (bitCount >> 1) & 1;
				bitCount = 0;

				uint64_t invKey = ~keyInt;
				key[0] &= invKey;
				key[1] &= invKey >> 16;
				key[2] &= invKey >> 32;
				key[3] &= invKey >> 48;
				invKey = 0;

				s.encrypt_block(plaintext, key, ciphertext);
				for (int s = 0; s < 16; ++s)
				{
					if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
					if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
				}
				output ^= (bitCount >> 1) & 1;
				bitCount = 0;
			}

			if (output == 1)
			{
				secretVariablesIndexes = 1ULL << i;
				output = 0;
				break;
			}
		}
	}

	return secretVariablesIndexes;
}

void CubeAttack::compute_quadratic_superpoly(uint32_t maxterm,
	uint64_t secretVariables, std::vector<std::vector<int>>& superpoly)
{
	int maxtermCount = 0;
	int secretVariablesCount = 0;
	std::vector<int> cubeIndexes = {};
	std::vector<int> secretVariablesIndexes = {};
	for (int i = 0; i < 32; ++i)
	{
		if (((maxterm >> i) & 1) == 1)
		{
			maxtermCount++;
			cubeIndexes.push_back(i);
		}
		if (((secretVariables >> i) & 1) == 1)
		{
			secretVariablesCount++;
			secretVariablesIndexes.push_back(i);
		}
		if (((secretVariables >> (i+32)) & 1) == 1)
		{
			secretVariablesCount++;
			secretVariablesIndexes.push_back(i);
		}
	}
	uint32_t cardialDegree = 1U << maxtermCount;

	Speck s;
	uint16_t plaintext[2]  = { 0x0, 0x0 };
	uint16_t ciphertext[2] = { 0x0, 0x0 };
	uint16_t nul[4]        = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t key[4]        = { 0x0, 0x0, 0x0, 0x0 };
	uint32_t pt            = 0x0;
	uint64_t keyInt        = 0x0;

	int output   = 0;
	int bitCount = 0;
	superpoly.clear();
	superpoly.push_back(std::vector<int>{});
	superpoly.push_back(std::vector<int>{});

	for (int r1 = 0; r1 < secretVariablesCount; ++r1)
	{
		keyInt = 1ULL << secretVariablesIndexes[r1];
		key[0] = keyInt;
		key[1] = keyInt >> 16;
		key[2] = keyInt >> 32;
		key[3] = keyInt >> 48;
		keyInt = 0;

		for (uint32_t j = 0; j < cardialDegree; ++j)
		{
			for (int b = 0; b < maxtermCount; ++b)
			{
				if ((j & (1U << b)) > 0)
					pt |= (1U << cubeIndexes[b]);
				else
					pt &= ~(1U << cubeIndexes[b]);
			}
			plaintext[0] = pt;
			plaintext[1] = pt >> 16;

			s.encrypt_block(plaintext, nul, ciphertext);
			for (int s = 0; s < 16; ++s)
			{
				if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
				if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
			}
			output ^= (bitCount >> 1) & 1;
			bitCount = 0;

			s.encrypt_block(plaintext, key, ciphertext);
			for (int s = 0; s < 16; ++s)
			{
				if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
				if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
			}
			output ^= (bitCount >> 1) & 1;
			bitCount = 0;
		}

		if (output == 1)
		{
			superpoly[0].push_back(secretVariablesIndexes[r1]);
			superpoly[1].push_back(0);
			output = 0;
		}
	}

	uint16_t key_01[4] = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t key_10[4] = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t key_11[4] = { 0x0, 0x0, 0x0, 0x0 };

	int sVCdec = secretVariablesCount - 1;
	for (int r2_1 = 0; r2_1 < sVCdec; ++r2_1)
	{
		for (int r2_2 = r2_1 + 1; r2_2 < secretVariablesCount; ++r2_2)
		{
			keyInt    = 1ULL << secretVariablesIndexes[r2_2];
			key_01[0] = keyInt;
			key_01[1] = keyInt >> 16;
			key_01[2] = keyInt >> 32;
			key_01[3] = keyInt >> 48;
			keyInt    = 0;

			keyInt    = 1ULL << secretVariablesIndexes[r2_1];
			key_10[0] = keyInt;
			key_10[1] = keyInt >> 16;
			key_10[2] = keyInt >> 32;
			key_10[3] = keyInt >> 48;
			keyInt    = 0;

			keyInt    = 1ULL << secretVariablesIndexes[r2_1];
			keyInt    = 1ULL << secretVariablesIndexes[r2_2];
			key_11[0] = keyInt;
			key_11[1] = keyInt >> 16;
			key_11[2] = keyInt >> 32;
			key_11[3] = keyInt >> 48;
			keyInt    = 0;

			for (uint32_t j = 0; j < cardialDegree; ++j)
			{
				for (int b = 0; b < maxtermCount; ++b)
				{
					if ((j & (1U << b)) > 0)
						pt |= (1U << cubeIndexes[b]);
					else
						pt &= ~(1U << cubeIndexes[b]);
				}
				plaintext[0] = pt;
				plaintext[1] = pt >> 16;

				s.encrypt_block(plaintext, nul, ciphertext);
				for (int s = 0; s < 16; ++s)
				{
					if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
					if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
				}
				output ^= (bitCount >> 1) & 1;
				bitCount = 0;

				s.encrypt_block(plaintext, key_01, ciphertext);
				for (int s = 0; s < 16; ++s)
				{
					if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
					if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
				}
				output ^= (bitCount >> 1) & 1;
				bitCount = 0;

				s.encrypt_block(plaintext, key_10, ciphertext);
				for (int s = 0; s < 16; ++s)
				{
					if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
					if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
				}
				output ^= (bitCount >> 1) & 1;
				bitCount = 0;

				s.encrypt_block(plaintext, key_11, ciphertext);
				for (int s = 0; s < 16; ++s)
				{
					if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
					if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
				}
				output ^= (bitCount >> 1) & 1;
				bitCount = 0;
			}

			if (output == 1)
			{
				superpoly[0].push_back(secretVariablesIndexes[r2_1]);
				superpoly[1].push_back(secretVariablesIndexes[r2_2]);
				output = 0;
			}
		}
	}

	for (uint32_t j = 0; j < cardialDegree; ++j)
	{
		for (int b = 0; b < maxtermCount; ++b)
		{
			if ((j & (1U << b)) > 0)
				pt |= (1U << cubeIndexes[b]);
			else
				pt &= ~(1U << cubeIndexes[b]);
		}
		plaintext[0] = pt;
		plaintext[1] = pt >> 16;

		s.encrypt_block(plaintext, nul, ciphertext);
		for (int s = 0; s < 16; ++s)
		{
			if (((ciphertext[0] >> s) & 1) == 1) { bitCount++; };
			if (((ciphertext[1] >> s) & 1) == 1) { bitCount++; };
		}
		output ^= (bitCount >> 1) & 1;
		bitCount = 0;
	}

	if (output == 1)
	{
		superpoly[0].push_back(1);
		superpoly[1].push_back(0);
	}
}

void CubeAttack::print_quadratic_superpoly(const std::vector<std::vector<int>>& superpoly)
{
	std::ostringstream ls;
	int superpoly0Size = superpoly[0].size();
	ls << "0";
	for (int i = 0; i < superpoly0Size; ++i)
	{
		if ((superpoly[0][i] != 0) & (superpoly[1][i] == 0))
		{
			if (i != superpoly0Size - 1)
				ls << "+x" << superpoly[0][i];
			else
				ls << "+1";
		}
		if (superpoly[1][i] != 0)
			ls << "+x" << superpoly[0][i] << "*x" << superpoly[1][i];
	}
	if(superpoly.capacity() > 2)
		std::cout << ls.str() << std::endl;
}
