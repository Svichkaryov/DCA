#include "stdafx.h"
#include "iostream"
#include "Ciphers/Speck.h"
#include "Utils/utils.h"
#include "Attack/DCA/DynamicCubeAttack.h"
#include "Attack/CA/CubeAttack.h"
#include "random"
#include <sstream>
#include "Attack/CA/CubeFormer.h"
#include <iterator>

int main()
{
	CubeAttack ca;
	//ca.preprocessing_phase();
	ca.user_mode();

	system("pause");
	return 0;
}

