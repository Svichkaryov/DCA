#include "stdafx.h"
#include "iostream"
#include "Attack/DCA/DynamicCubeAttack.h"
#include "Attack/CA/CubeAttack.h"
#include "Ciphers/Simeck.h"

int main()
{
	CubeAttack ca;
	ca.preprocessing_phase();

	//ca.set_cubes({ 3909222913, 3909091845,3791929857, 3775009281, 3774878213, 3766748673, 3766487585, 3762554369, });
	//ca.set_cubes({ 3775009281, 3762554369});
	//ca.set_cubes({ 3766487585});
	//ca.online_phase();

	//ca.user_mode();
	
	system("pause");
	return 0;
}

