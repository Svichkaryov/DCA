#include "stdafx.h"
#include "iostream"
#include "Attack/DCA/DynamicCubeAttack.h"
#include "Attack/CA/CubeAttack.h"


int main()
{
	CubeAttack ca;
	ca.preprocessing_phase();

	//ca.set_cubes({ 5439552 });
	//ca.online_phase();

	//ca.user_mode();
	
	system("pause");
	return 0;
}

