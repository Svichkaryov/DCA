#include "stdafx.h"
#include "iostream"
#include "Ciphers/Speck.h"
#include "Utils/utils.h"
#include "Attack/DCA/DynamicCubeAttack.h"
#include "Attack/CA/CubeAttack.h"
#include "random"
#include <sstream>
#include "Attack/CA/CubeFormer.h"


int main()
{
	CubeAttack ca;
	ca.preprocessing_phase();
	//CubeFormer cf;
	//std::cout << cf.next_cube(2952790016);

	system("pause");
    return 0;
}

