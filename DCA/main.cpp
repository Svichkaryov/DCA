#include "stdafx.h"
#include "iostream"
#include "Attack/DCA/DynamicCubeAttack.h"
#include "Attack/CA/CubeAttack.h"
#include "Ciphers/Simeck.h"
#include "Utils/utils.h"



int main()
{
	CubeAttack ca;
	//ca.preprocessing_phase();
	
	//ca.set_cubes({ 3909222913, 3909091845,3791929857, 3775009281, 3774878213, 3766748673, 3766487585, 3762554369, });
	//ca.set_cubes({ 3775009281, 3762554369});
	
	//ca.set_extended_cubes({ 1098910208, 1098908224, 1091600640, 1090552065, 1090521856, 8777, 74280, 74312, 78344, 270418 }, 3); // 7 round -> 8 (cube size: 5+3)
	//ca.set_extended_cubes({1090831105, 1627423497, 1091046168, 1091308304, 17904168, 17916456, 554770984, 17904168}, 6); // speck/9round/0bit/13deg/
	//ca.set_extended_cubes({ 7 }, 1);
	//ca.online_phase();

	ca.user_mode(MAXTERM_FORM::INT_FORM);

	system("pause");
	return 0;
}

