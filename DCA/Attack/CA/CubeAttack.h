#pragma once
#include <vector>
#include "CubeFormer.h"
#include "../../Ciphers/Speck.h"


class CubeAttack
{
public:
	CubeAttack();
	~CubeAttack() = default;

	void preprocessing_phase();
	void online_phase();
	void user_mode();

	bool linear_test(uint32_t maxterm);
	void compute_linear_superpoly(uint32_t maxterm, uint64_t superpoly[2]);
	void print_linear_superpoly(uint32_t maxterm, const uint64_t superpoly[2]);
	
	bool quadratic_test(uint32_t maxterm);
	uint64_t find_secret_variables(uint32_t maxterm);
	void compute_quadratic_superpoly(uint32_t maxterm, 
		uint64_t secretVariables, std::vector<std::vector<int>>& superpoly);
	void print_quadratic_superpoly(uint32_t maxterm, const std::vector<std::vector<int>>& superpoly);

private:
	CubeFormer cubeFormer;
	Speck speckCipher;
};