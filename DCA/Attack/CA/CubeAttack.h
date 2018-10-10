#pragma once
#include <vector>


class CubeAttack
{
public:
	CubeAttack() = default;
	~CubeAttack() = default;

	void preprocessing_phase();
	void online_phase();

	void user_mode();

	bool linear_test(uint32_t maxterm);
	void compute_superpoly(uint32_t maxterm, uint64_t superpoly[2]);
	void print_linear_superpoly(const uint64_t superpoly[2]);


private:
};
