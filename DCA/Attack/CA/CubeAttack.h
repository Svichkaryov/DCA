#pragma once
#include <vector>
#include <fstream>
#include "CubeFormer.h"
#include "../../Ciphers/Cipher_32_64.h"


enum class MAXTERM_FORM { INT_FORM, INDEX_FORM};
enum class Ciphers { SIMECK_32_64, SPECK_32_64, SIMON_32_64 };

class CubeAttack
{
public:
	CubeAttack();
	CubeAttack(Cipher_32_64* p_cipher);
	~CubeAttack();

	void set_settings(Ciphers cipher, int round, OutputStateStategy oss, int oss_param);
	void free_setting();

	void preprocessing_phase();
	void online_phase();
	void user_mode(MAXTERM_FORM mf);

	bool linear_test(uint32_t maxterm);
	bool linear_test_blr(uint32_t maxterm);
	bool linear_test_tbt(uint32_t maxterm);
	void compute_linear_superpoly(uint32_t maxterm, uint64_t superpoly[2]);
	void print_linear_superpoly(uint32_t maxterm, const uint64_t superpoly[2], int output = -1);
	
	bool quadratic_test(uint32_t maxterm);
	uint64_t find_secret_variables(uint32_t maxterm);
	void compute_quadratic_superpoly(uint32_t maxterm, 
		uint64_t secretVariables, std::vector<std::vector<int>>& superpoly);
	void print_quadratic_superpoly(uint32_t maxterm, 
		const std::vector<std::vector<int>>& superpoly, int output = -1);

	void set_cubes(std::initializer_list<uint32_t> cubes);
	void set_extended_cubes(std::initializer_list<uint32_t> cubes, int extendedDimension);

private:
	Cipher_32_64* m_cipher;
	CubeFormer cubeFormer;

	using linear_test_t = bool(CubeAttack::*)(uint32_t); 
	linear_test_t p_linearTest;

	int n_linearTest;
	int n_quadraticTest;
	int n_randSamplesForSVI;
	std::vector<uint32_t> cubesSet;

	std::ofstream m_out;
};