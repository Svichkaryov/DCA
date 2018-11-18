#include "stdafx.h"
#include "CubeAttack.h"
#include "CubeAttackManager.h"


void CubeAttackManager::cube_attack_run(Cipher_32_64* cipher)
{
	CubeAttack ca(cipher);
	ca.preprocessing_phase();
}