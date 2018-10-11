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
	ca.user_mode();

	//uint32_t pt = 0;

	//int maxtermCount = 0;
	//std::vector<int> cubeIndexes = {};

	//for (int i = 0; i < 32; ++i)
	//{
	//	if (((7 >> i) & 1) == 1)
	//	{
	//		maxtermCount++;
	//		cubeIndexes.push_back(i);
	//	}
	//}

	//uint32_t cardialDegree = 1U << maxtermCount;

	//for (uint32_t j = 0; j < cardialDegree; ++j)
	//{
	//	for (int b = 0; b < maxtermCount; ++b)
	//	{
	//		if ((j & (1U << b)) > 0)
	//			pt |= (1U << cubeIndexes[b]);
	//		else
	//			pt &= ~(1U << cubeIndexes[b]);
	//	}
	//	std::cout << pt << "\n";
	//}


	system("pause");
    return 0;
}

