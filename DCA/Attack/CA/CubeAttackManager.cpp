#include "stdafx.h"
#include <thread>
#include "CubeAttack.h"
#include "CubeAttackManager.h"


void CubeAttackManager::threadFunc(CipherARX_32_64* cipher)
{
	CubeAttack ca(cipher);
	ca.preprocessing_phase();
}