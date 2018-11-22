#include "stdafx.h"
#include "CAWrap.h"
#include "sstream"

CAWrap::~CAWrap()
{
	ca.free_setting();
}

void CAWrap::settings(int cipherId, int roundNum, int ossId, int ossParam)
{
	ca.free_setting();
	if(cipherId == 1 && ossId == 1)
		ca.set_settings(Ciphers::SIMECK_32_64, roundNum, OutputStateStategy::HW, ossParam);
	else if(cipherId == 1 && ossId == 2)
		ca.set_settings(Ciphers::SIMECK_32_64, roundNum, OutputStateStategy::HW_BYTE, ossParam);
	else if(cipherId == 1 && ossId == 3)
		ca.set_settings(Ciphers::SIMECK_32_64, roundNum, OutputStateStategy::RAW_STATE, ossParam);
	else if(cipherId == 2 && ossId == 1)
		ca.set_settings(Ciphers::SIMON_32_64, roundNum, OutputStateStategy::HW, ossParam);
	else if(cipherId == 2 && ossId == 2)
		ca.set_settings(Ciphers::SIMON_32_64, roundNum, OutputStateStategy::HW_BYTE, ossParam);
	else if(cipherId == 2 && ossId == 3)
		ca.set_settings(Ciphers::SIMON_32_64, roundNum, OutputStateStategy::RAW_STATE, ossParam);
	else if (cipherId == 3 && ossId == 1)
		ca.set_settings(Ciphers::SPECK_32_64, roundNum, OutputStateStategy::HW, ossParam);
	else if (cipherId == 3 && ossId == 2)
		ca.set_settings(Ciphers::SPECK_32_64, roundNum, OutputStateStategy::HW_BYTE, ossParam);
	else if (cipherId == 3 && ossId == 3)
		ca.set_settings(Ciphers::SPECK_32_64, roundNum, OutputStateStategy::RAW_STATE, ossParam);
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
	else if (ca.quadratic_test(cube))
	{
		ca.compute_quadratic_superpoly(cube,
			ca.find_secret_variables(cube), quadratic_superpoly);
		ca.print_quadratic_superpoly(cube, quadratic_superpoly);
	}
	else
	{
		std::ostringstream ls;

		ls << "Cube : { ";
		for (int i = 0; i < 32; ++i)
		{
			if (((cube >> i) & 1) == 1)
				ls << i << " ";
		}
		ls << "}\n";
		printf("No superpoly for %s", ls.str().c_str());
	}
}
