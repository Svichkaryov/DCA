#include "stdafx.h"
#include "Ciphers/Speck.h"
#include "Utils/utils.h"
#include "Attack/DCA/DynamicCubeAttack.h"
#include "Attack/CA/CubeAttack.h"
#include "random"


int main()
{
	CubeAttack ca;

	std::cout << ca.linear_test({ 31, 14 });
	auto aeweeq = ca.compute_superpoly({ 31, 14 });
	std::cout << "\n";
	for (auto& el : aeweeq)
	{
		std::cout << el;
	}

	std::cout << std::endl;
	system("pause");
    return 0;
}

