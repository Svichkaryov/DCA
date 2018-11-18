#include "stdafx.h"
#include "random"
#include "sstream"
#include <thread>
#include "CubeAttack.h"
#include "../../Ciphers/Speck.h"
#include "../../Ciphers/Simeck.h"
#include "../../Ciphers/Simon.h"
#include "CubeFormer.h"


//#define ONLINE_PHASE
//#define DOUBLE_CHECK
#define LINEAR_SEARCH
#define QUADRATIC_SEARCH

//#define CONSOLE_PRINT_SUPERPOLY
#define FILE_PRINT_SUPERPOLY


CubeAttack::CubeAttack()
{
	//Speck* cipher = new Speck(OutputStateStategy::HW, 0x2);
	//Simon* cipher = new Simon(OutputStateStategy::RAW_STATE, 1);
	Simeck* cipher = new Simeck(OutputStateStategy::HW, 0x00); 
	m_cipher = cipher;
	p_linearTest = &CubeAttack::linear_test_blr;
	n_linearTest = 100;
	n_quadraticTest = 100;
	n_randSamplesForSVI = 50;

#ifdef FILE_PRINT_SUPERPOLY

	std::string filePath = "Attack/CA/result/";
	std::string fileName = m_cipher->cipher_info() + std::string("_") +
		m_cipher->get_outputStateStategy_name() + std::string("_") +
		std::to_string(m_cipher->get_nBitOutput()) + ".txt";
	m_out.open(filePath + fileName);

	if (m_out.fail())
	{
		throw std::invalid_argument("Unable to open file");
	}
	m_out << "File consist cube indexes with corresponding superpoly for " << 
		m_cipher->cipher_info() << " cipher. " <<
		"Output state strategy: " << m_cipher->get_outputStateStategy_name() << ". " <<
		"Ouput bit number is: " << std::to_string(m_cipher->get_nBitOutput()) << "\n";
	m_out << "-------------------------------------------------------------------------------------\n";

#endif // FILE_PRINT_SUPERPOLY
}

CubeAttack::CubeAttack(Cipher_32_64* p_cipher)
{
	m_cipher = p_cipher;
	p_linearTest = &CubeAttack::linear_test_blr;
	n_linearTest = 100;
	n_quadraticTest = 100;
	n_randSamplesForSVI = 50;

#ifdef FILE_PRINT_SUPERPOLY

	std::string filePath = "Attack/CA/result/";
	std::string fileName = m_cipher->cipher_info() + std::string("_") +
		m_cipher->get_outputStateStategy_name() + std::string("_") +
		std::to_string(m_cipher->get_nBitOutput()) + ".txt";
	m_out.open(filePath + fileName);

	if (m_out.fail())
	{
		throw std::invalid_argument("Unable to open file");
	}
	m_out << "File consist cube indexes with corresponding superpoly for " <<
		m_cipher->cipher_info() << " cipher. " <<
		"Output state strategy: " << m_cipher->get_outputStateStategy_name() << ". " <<
		"Ouput bit number is: " << std::to_string(m_cipher->get_nBitOutput()) << "\n";
	m_out << "-------------------------------------------------------------------------------------\n";

#endif FILE_PRINT_SUPERPOLY
}

CubeAttack::~CubeAttack()
{
	if (m_out.is_open())
	{
		m_out.close();
	}
	delete m_cipher;
}

void CubeAttack::preprocessing_phase()
{
	int cubeDim = 5;
	int cubeCount = cubeFormer.get_end_flag(cubeDim);
	//uint32_t startCube = cubeFormer.get_start_cube(cubeDim);
	uint32_t startCube = cubeFormer.get_end_cube(cubeDim);
	uint32_t nextCube = startCube;

	uint64_t linear_superpoly[2];
	std::vector<std::vector<int>> quadratic_superpoly(2);
	int count = 0;
	std::vector<uint64_t> linSuperpoly = {};

	while (count != cubeCount)
	{
#ifdef LINEAR_SEARCH
		if (linear_test(nextCube))
		{
			compute_linear_superpoly(nextCube, linear_superpoly);
			print_linear_superpoly(nextCube, linear_superpoly);

#ifdef ONLINE_PHASE
			if (std::find(linSuperpoly.begin(), linSuperpoly.end(), linear_superpoly[0]) == linSuperpoly.end())
			{
				linSuperpoly.push_back(linear_superpoly[0]);
				cubesSet.push_back(nextCube);
			}
#endif // ONLINE_PHASE

		}
		else
		{
#endif // LINEAR_SEARCH

#ifdef QUADRATIC_SEARCH
			if (quadratic_test(nextCube))
			{
				compute_quadratic_superpoly(nextCube,
					find_secret_variables(nextCube), quadratic_superpoly);
				print_quadratic_superpoly(nextCube, quadratic_superpoly);
			}
#endif // QUADRATIC_SEARCH_F

#ifdef LINEAR_SEARCH
		}
#endif // LINEAR_SEARCH

		count++;
		if (count % 1'000'000 == 0)
			printf("%d cube viewed for thread with id: %d\n", count, std::this_thread::get_id());

		//nextCube = cubeFormer.next_cube(nextCube);
		nextCube = cubeFormer.prev_cube(nextCube);
		//nextCube = cubeFormer.rand_cube();
		//nextCube = cubeFormer.rand_cube(5, 6);
	}
	//std::cout << "Count = " << count << std::endl;
}

void CubeAttack::online_phase()
{
	uint16_t plaintext[2]  = { 0x0, 0x0 };
	uint16_t ciphertext[2] = { 0x0, 0x0 };
	uint16_t key[2]        = { 0x2, 0x2 };
	uint16_t nul[4]        = { 0x0, 0x0, 0x0, 0x0 };
	uint32_t pt            = { 0x0 };
	uint64_t linear_superpoly[2];
	std::vector<std::vector<int>> quadratic_superpoly;

	int output;
	int maxtermCount;
	std::vector<int> cubeIndexes = {};

	for (auto el : cubesSet)
	{
		cubeIndexes.clear();
		maxtermCount = 0;
		output = 0;
		pt = 0x0;
		for (int i = 0; i < 32; ++i)
		{
			if (((el >> i) & 1) == 1)
				cubeIndexes.push_back(i);
		}
		maxtermCount = cubeIndexes.size();
		uint32_t cardialDegree = 1U << maxtermCount;

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

			m_cipher->encrypt_block(plaintext, key, ciphertext);
			output ^= m_cipher->get_bit(ciphertext);
		}

		if (linear_test(el))
		{
			compute_linear_superpoly(el, linear_superpoly);
			print_linear_superpoly(el, linear_superpoly, output);
		}
		else if (quadratic_test(el))
		{
			compute_quadratic_superpoly(el,
				find_secret_variables(el), quadratic_superpoly);
			print_quadratic_superpoly(el, quadratic_superpoly, output);
		}
	}
}

void CubeAttack::user_mode(MAXTERM_FORM mf)
{
	char action;
	uint32_t maxterm;
	uint64_t linear_superpoly[2];
	std::vector<std::vector<int>> quadratic_superpoly;
	do
	{
		maxterm = 0x0;

		if (mf == MAXTERM_FORM::INDEX_FORM)
		{
			uint32_t index;
			const int end = '.'; // = 46 = 0x2E

			printf("Input cube indexes(end with \'.\'): ");

			for (; std::cin >> index && index != end;)
			{
				maxterm |= 1U << (index);
			}

			if (!std::cin)
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				printf("Error input\n");
			}
		}
		else if (mf == MAXTERM_FORM::INT_FORM)
		{
			printf("Input maxterm: ");

			std::cin >> maxterm;

			if (!std::cin)
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				printf("Error input\n");
			}
		}

		if (linear_test(maxterm))
		{
			std::cout << "lin\n";
			compute_linear_superpoly(maxterm, linear_superpoly);
			print_linear_superpoly(maxterm, linear_superpoly);
		}
		else
		{
			if (quadratic_test(maxterm))
			{
				std::cout << "quadra\n";
				compute_quadratic_superpoly(maxterm,
					find_secret_variables(maxterm), quadratic_superpoly);
				print_quadratic_superpoly(maxterm, quadratic_superpoly);
			}
		}
		printf("action(Next(n)/Exit(e)): ");
		std::cin >> action;
	} while (action != 'e');
}

bool CubeAttack::linear_test(uint32_t maxterm)
{
	return (this->*p_linearTest)(maxterm);
}

bool CubeAttack::linear_test_blr(uint32_t maxterm)
{
	uint16_t plaintext[2]  = { 0x0, 0x0 };
	uint16_t ciphertext[2] = { 0x0, 0x0 };
	uint16_t x[4]          = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t y[4]          = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t xy[4]         = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t nul[4]        = { 0x0, 0x0, 0x0, 0x0 };
	uint32_t pt            = { 0x0 };

	uint64_t rand = 0;
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<uint64_t> dis(0x1, 0xFFFFFFFFFFFFFFFF);
	
	int maxtermCount;
	std::vector<int> cubeIndexes = {};
	for (int i = 0; i < 32; ++i)
	{
		if (((maxterm >> i) & 1) == 1)
		{
			cubeIndexes.push_back(i);
		}
	}
	maxtermCount = cubeIndexes.size();
	uint32_t cardialDegree = 1U << maxtermCount;

	int answer = 0;
	for (int i = 0; i < n_linearTest; ++i)
	{
		//x[0] = dis(gen);
		//x[1] = dis(gen);
		//x[2] = dis(gen);
		//x[3] = dis(gen);

		//y[0] = dis(gen);
		//y[1] = dis(gen);
		//y[2] = dis(gen);
		//y[3] = dis(gen);

		rand = gen();
		x[0] = rand;
		x[1] = rand >> 16;
		x[2] = rand >> 32;
		x[3] = rand >> 48;

		rand = gen();
		y[0] = rand;
		y[1] = rand >> 16;
		y[2] = rand >> 32;
		y[3] = rand >> 48;

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

			m_cipher->encrypt_block(plaintext, x, ciphertext);
			answer ^= m_cipher->get_bit(ciphertext);
				
			m_cipher->encrypt_block(plaintext, y, ciphertext);
			answer ^= m_cipher->get_bit(ciphertext);

			m_cipher->encrypt_block(plaintext, xy, ciphertext);
			answer ^= m_cipher->get_bit(ciphertext);

			m_cipher->encrypt_block(plaintext, nul, ciphertext);
			answer ^= m_cipher->get_bit(ciphertext);
		}
		if (answer == 1) return false;
		answer = 0;
	}

	return true;
}

bool CubeAttack::linear_test_tbt(uint32_t maxterm)
{
	uint16_t plaintext[2]  = { 0x0, 0x0 };
	uint16_t ciphertext[2] = { 0x0, 0x0 };
	uint16_t x[4]          = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t y[4]          = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t z[4]          = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t nul[4]        = { 0x0, 0x0, 0x0, 0x0 };
	uint32_t pt            = { 0x0 };
	uint64_t keyInt        = { 0x0 };
	uint64_t invKeyInt     = { 0x0 };

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<uint16_t> dis(0x0, 0xFFFF);

	int maxtermCount = 0;
	std::vector<int> cubeIndexes = {};
	std::vector<int> keyIndexes = {};
	for (int i = 0; i < 32; ++i)
	{
		if (((maxterm >> i) & 1) == 1)
		{
			cubeIndexes.push_back(i);
		}
	}
	maxtermCount = cubeIndexes.size();
	uint32_t cardialDegree = 1U << maxtermCount;

	int res = 0;
	for (int k = 0; k < 64; ++k)
	{
		keyInt = 1ULL << k;
		x[0] = keyInt;
		x[1] = keyInt >> 16;
		x[2] = keyInt >> 32;
		x[3] = keyInt >> 48;

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

			m_cipher->encrypt_block(plaintext, x, ciphertext);
			res ^= m_cipher->get_bit(ciphertext);

			m_cipher->encrypt_block(plaintext, nul, ciphertext);
			res ^= m_cipher->get_bit(ciphertext);
		}
		if (res == 1)
			keyIndexes.push_back(k);
		res = 0;
	}

	for (auto el : keyIndexes)
	{
		for (int i = 0; i < n_linearTest; ++i)
		{
			res = 0;

			y[0] = dis(gen);
			y[1] = dis(gen);
			y[2] = dis(gen);
			y[3] = dis(gen);

			z[0] = y[0];
			z[1] = y[1];
			z[2] = y[2];
			z[3] = y[3];

			keyInt = 1ULL << el;
			y[0] |= keyInt;
			y[1] |= keyInt >> 16;
			y[2] |= keyInt >> 32;
			y[3] |= keyInt >> 48;

			invKeyInt = ~keyInt;
			z[0] &= invKeyInt;
			z[1] &= invKeyInt >> 16;
			z[2] &= invKeyInt >> 32;
			z[3] &= invKeyInt >> 48;

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

				m_cipher->encrypt_block(plaintext, y, ciphertext);
				res ^= m_cipher->get_bit(ciphertext);

				m_cipher->encrypt_block(plaintext, z, ciphertext);
				res ^= m_cipher->get_bit(ciphertext);
			}
			if (res == 0) return false;
		}
	}
	return true;
}

void CubeAttack::compute_linear_superpoly(uint32_t maxterm, uint64_t superpoly[2])
{
	uint16_t plaintext[2]  = { 0x0, 0x0 };
	uint16_t ciphertext[2] = { 0x0, 0x0 };
	uint16_t key[4]        = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t nul[4]        = { 0x0, 0x0, 0x0, 0x0 };
	uint32_t pt            = { 0x0 };
	uint64_t keyInt        = { 0x0 };
	superpoly[0]           = { 0x0 };
	superpoly[1]           = { 0x0 };

	std::vector<int> cubeIndexes = {};
	for (int i = 0; i < 32; ++i)
	{
		if (((maxterm >> i) & 1) == 1)
		{
			cubeIndexes.push_back(i);
		}
	}
	int maxtermCount = cubeIndexes.size();
	uint32_t cardialDegree = 1U << maxtermCount;
	
	int constant = 0;
	int coeff = 0;

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

		m_cipher->encrypt_block(plaintext, nul, ciphertext);
		constant ^= m_cipher->get_bit(ciphertext);
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

			m_cipher->encrypt_block(plaintext, key, ciphertext);
			coeff ^= m_cipher->get_bit(ciphertext);
		}
		if ((constant ^ coeff) == 1)
			superpoly[0] |= 1ULL << k;

		coeff = 0;
	}
}

void CubeAttack::print_linear_superpoly(uint32_t maxterm, const uint64_t superpoly[2], int output)
{
#ifdef DOUBLE_CHECK
	for (int i = 0; i < 10; i++)
	{
		if (!linear_test(maxterm))
			return;
	}
#endif // DOUBLE_CHECK
	
	if (superpoly[0] > 0)
	{
		std::ostringstream ls;

		ls << "Cube : { ";
		for (int i = 0; i < 32; ++i)
		{
			if (((maxterm >> i) & 1) == 1)
				ls << i << " ";
		}
		ls << "} ~ " << maxterm << "\n";

		ls << "Superpoly : " << superpoly[1];

		for (int i = 0; i < 64; ++i)
		{
			if (((superpoly[0] >> i) & 1) == 1)
				ls << "+x" << i;
		}
		ls << "\n";

#ifdef CONSOLE_PRINT_SUPERPOLY
		std::cout << ls.str();

		if (output != -1)
			std::cout << "Output = " << output << std::endl;
	
#endif // CONSOLE_PRINT_SUPERPOLY

#ifdef FILE_PRINT_SUPERPOLY
		m_out << ls.str();
		
		if (output != -1)
			m_out << "Output = " << output << "\n";
#endif // FILE_PRINT_SUPERPOLY
	}
}

bool CubeAttack::quadratic_test(uint32_t maxterm)
{
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
	uint32_t pt            = { 0x0 };

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<uint16_t> dis(0x0, 0xFFFF);

	std::vector<int> cubeIndexes = {};
	for (int i = 0; i < 32; ++i)
	{
		if (((maxterm >> i) & 1) == 1)
		{
			cubeIndexes.push_back(i);
		}
	}
	int maxtermCount = cubeIndexes.size();
	uint32_t cardialDegree = 1U << maxtermCount;
	
	int answer = 0;
	for (int i = 0; i < n_quadraticTest; ++i)
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

			m_cipher->encrypt_block(plaintext, x, ciphertext);
			answer ^= m_cipher->get_bit(ciphertext);

			m_cipher->encrypt_block(plaintext, y, ciphertext);
			answer ^= m_cipher->get_bit(ciphertext);

			m_cipher->encrypt_block(plaintext, z, ciphertext);
			answer ^= m_cipher->get_bit(ciphertext);

			m_cipher->encrypt_block(plaintext, xy, ciphertext);
			answer ^= m_cipher->get_bit(ciphertext);

			m_cipher->encrypt_block(plaintext, xz, ciphertext);
			answer ^= m_cipher->get_bit(ciphertext);

			m_cipher->encrypt_block(plaintext, yz, ciphertext);
			answer ^= m_cipher->get_bit(ciphertext);

			m_cipher->encrypt_block(plaintext, xyz, ciphertext);
			answer ^= m_cipher->get_bit(ciphertext);

			m_cipher->encrypt_block(plaintext, nul, ciphertext);
			answer ^= m_cipher->get_bit(ciphertext);
		}
		if (answer == 1) return false;
		answer = 0;
	}

	return true;
}

uint64_t CubeAttack::find_secret_variables(uint32_t maxterm)
{
	uint16_t plaintext[2]  = { 0x0, 0x0 };
	uint16_t ciphertext[2] = { 0x0, 0x0 };
	uint16_t key[4]        = { 0x0, 0x0, 0x0, 0x0 };
	uint32_t pt            = { 0x0 };
	uint64_t keyInt        = { 0x0 };
	uint64_t invKey        = { 0x0 };
	
	uint64_t rand = 0;
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<uint64_t> dis(0x1, 0xFFFFFFFFFFFFFFFF);

	std::vector<int> cubeIndexes = {};
	for (int i = 0; i < 32; ++i)
	{
		if (((maxterm >> i) & 1) == 1)
			cubeIndexes.push_back(i);
	}
	int maxtermCount = cubeIndexes.size();
	uint32_t cardialDegree = 1U << maxtermCount;

	int output = 0;
	uint64_t secretVariablesIndexes = 0x0;

	for (int i = 0; i < 64; ++i)
	{
		keyInt = 1ULL << i;
		invKey = ~keyInt;

		for (int k = 0; k < n_randSamplesForSVI; ++k)
		{
			rand = gen();
			key[0] = rand;
			key[1] = rand >> 16;
			key[2] = rand >> 32;
			key[3] = rand >> 48;

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

				key[0] |= keyInt;
				key[1] |= keyInt >> 16;
				key[2] |= keyInt >> 32;
				key[3] |= keyInt >> 48;

				m_cipher->encrypt_block(plaintext, key, ciphertext);
				output ^= m_cipher->get_bit(ciphertext);

				key[0] &= invKey;
				key[1] &= (invKey >> 16);
				key[2] &= (invKey >> 32);
				key[3] &= (invKey >> 48);
				
				m_cipher->encrypt_block(plaintext, key, ciphertext);
				output ^= m_cipher->get_bit(ciphertext);
			}

			if (output == 1)
			{
				secretVariablesIndexes |= keyInt;
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
	uint16_t plaintext[2]  = { 0x0, 0x0 };
	uint16_t ciphertext[2] = { 0x0, 0x0 };
	uint16_t nul[4]        = { 0x0, 0x0, 0x0, 0x0 };
	uint16_t key[4]        = { 0x0, 0x0, 0x0, 0x0 };
	uint32_t pt            = { 0x0 };
	uint64_t keyInt        = { 0x0 };

	superpoly[0].clear();
	superpoly[1].clear();
	
	std::vector<int> cubeIndexes = {};
	std::vector<int> secretVariablesIndexes = {};
	for (int i = 0; i < 32; ++i)
	{
		if (((maxterm >> i) & 1) == 1)
			cubeIndexes.push_back(i);

		if (((secretVariables >> i) & 1) == 1)
			secretVariablesIndexes.push_back(i);
		
		if (((secretVariables >> (i+32)) & 1) == 1)
			secretVariablesIndexes.push_back(i+32);
	}
	int maxtermCount = cubeIndexes.size();
	int secretVariablesCount = secretVariablesIndexes.size();
	uint32_t cardialDegree = 1U << maxtermCount;

	int output = 0;

	for (int r1 = 0; r1 < secretVariablesCount; ++r1)
	{
		keyInt = 1ULL << secretVariablesIndexes[r1];
		key[0] = keyInt;
		key[1] = keyInt >> 16;
		key[2] = keyInt >> 32;
		key[3] = keyInt >> 48;

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

			m_cipher->encrypt_block(plaintext, nul, ciphertext);
			output ^= m_cipher->get_bit(ciphertext);

			m_cipher->encrypt_block(plaintext, key, ciphertext);
			output ^= m_cipher->get_bit(ciphertext);
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

			keyInt    = 1ULL << secretVariablesIndexes[r2_1];
			key_10[0] = keyInt;
			key_10[1] = keyInt >> 16;
			key_10[2] = keyInt >> 32;
			key_10[3] = keyInt >> 48;

			keyInt    = 1ULL << secretVariablesIndexes[r2_1];
			keyInt   |= 1ULL << secretVariablesIndexes[r2_2];
			key_11[0] = keyInt;
			key_11[1] = keyInt >> 16;
			key_11[2] = keyInt >> 32;
			key_11[3] = keyInt >> 48;

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

				m_cipher->encrypt_block(plaintext, nul, ciphertext);
				output ^= m_cipher->get_bit(ciphertext);

				m_cipher->encrypt_block(plaintext, key_01, ciphertext);
				output ^= m_cipher->get_bit(ciphertext);

				m_cipher->encrypt_block(plaintext, key_10, ciphertext);
				output ^= m_cipher->get_bit(ciphertext);

				m_cipher->encrypt_block(plaintext, key_11, ciphertext);
				output ^= m_cipher->get_bit(ciphertext);
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

		m_cipher->encrypt_block(plaintext, nul, ciphertext);
		output ^= m_cipher->get_bit(ciphertext);
	}
	superpoly[0].push_back(output);
	superpoly[1].push_back(0);	
}

void CubeAttack::print_quadratic_superpoly(uint32_t maxterm, 
	const std::vector<std::vector<int>>& superpoly, int output)
{
#ifdef DOUBLE_CHECK
	for (int i = 0; i < 10; i++)
	{
		if (!quadratic_test(maxterm))
			return;
	}
#endif // DOUBLE_CHECK

	bool f_deg2 = false;
	std::ostringstream ls2;

	if (superpoly[0].size() > 0)
	{
		int superpoly0Size = superpoly[0].size() - 1;
		ls2 << "Superpoly : " << superpoly[0][superpoly0Size];

		for (int i = 0; i < superpoly0Size; ++i)
		{
			if ((superpoly[0][i] != 0) & (superpoly[1][i] == 0))
				ls2 << "+x" << superpoly[0][i];
			if (superpoly[1][i] != 0)
			{
				f_deg2 = true;
				ls2 << "+x" << superpoly[0][i] << "*x" << superpoly[1][i];
			}
		}
	}
	
	if (superpoly[1].size() > 1 && f_deg2 == true)
	{
		std::ostringstream ls;

		ls << "Cube : { ";
		for (int i = 0; i < 32; ++i)
		{
			if (((maxterm >> i) & 1) == 1)
				ls << i << " ";
		}
		ls << "} ~ " << maxterm << "\n";
		ls << ls2.str() << "\n";

#ifdef CONSOLE_PRINT_SUPERPOLY
		std::cout << ls.str();

		if (output != -1)
			std::cout << "Output = " << output << std::endl;
#endif // CONSOLE_PRINT_SUPERPOLY

#ifdef FILE_PRINT_SUPERPOLY
		m_out << ls.str();

		if (output != -1)
			m_out << "Output = " << output << "\n";
#endif // FILE_PRINT_SUPERPOLY
	}
}

void CubeAttack::set_cubes(std::initializer_list<uint32_t> cubes)
{
	cubesSet.insert(cubesSet.end(), cubes.begin(), cubes.end());
}

void CubeAttack::set_extended_cubes(std::initializer_list<uint32_t> cubes, int extendedDimension)
{
	for (auto el : cubes)
	{
		cubesSet.push_back(el);

		bool f_subCube = false;
		int count = 0;
		int cubeCount = cubeFormer.get_end_flag(extendedDimension);
		uint32_t startCube = cubeFormer.get_start_cube(extendedDimension);
		uint32_t nextCube = startCube;
		uint32_t extendedCube = el;
		
		while (count != cubeCount)
		{
			for (int i = 0; i < 32; ++i)
			{
				if (((nextCube >> i) & 1) == 1)
				{
					if (((extendedCube >> i) & 1) != 1)
						extendedCube |= 1U << i;
					else
						f_subCube = true;
				}
			}
			count++;
			nextCube = cubeFormer.next_cube(nextCube);
			if (!f_subCube)
			{
				cubesSet.push_back(extendedCube);
				//cubesSet.push_back(extendedCube >> 1);
			}
			f_subCube = false;
			extendedCube = el;
		}
	}
}