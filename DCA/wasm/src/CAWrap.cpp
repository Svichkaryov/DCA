#include "stdafx.h"
#include "CAWrap.h"


CAWrap::~CAWrap()
{
	ca.free_setting();
}

void CAWrap::settings(int cipherId, int roundNum, int ossId, int ossParam)
{
	ca.free_setting();
	ca.set_settings(Ciphers::SIMECK_32_64, 4, OutputStateStategy::HW, 0);
}

void CAWrap::user_mode(uint32_t cube)
{
	uint64_t linear_superpoly[2];
	std::vector<std::vector<int>> quadratic_superpoly;

	if (ca.linear_test(cube))
	{
		ca.compute_linear_superpoly(cube, linear_superpoly);
		ca.print_linear_superpoly(cube, linear_superpoly);
	}
	else
	{
		if (ca.quadratic_test(cube))
		{
			ca.compute_quadratic_superpoly(cube,
				ca.find_secret_variables(cube), quadratic_superpoly);
			ca.print_quadratic_superpoly(cube, quadratic_superpoly);
		}
	}
}
