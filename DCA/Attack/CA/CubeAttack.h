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

	bool linear_test(std::initializer_list<int> maxterm);
	std::vector<int> compute_superpoly(std::initializer_list<int> maxterm);
private:

};
