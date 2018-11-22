#pragma once
#include "../../Attack/CA/CubeAttack.h"


class CAWrap
{
public:
	CAWrap() = default;
	~CAWrap();
	
	void settings(int cipherId, int roundNum, int ossId, int ossParam);
	void user_mode(uint32_t cube);

private:
	CubeAttack ca;
};