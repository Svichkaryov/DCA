#include "CAWrap.h"
#include "../../Attack/CA/CubeAttack.h"
#include "../../Ciphers/Simeck.h"
#include "../../Ciphers/Speck.h"
#include "../../Ciphers/Simon.h"
#include <emscripten/emscripten.h>

CAWrap caw;

int main()
{
	//caw.settings(1, 4, 1, 0);
	printf("Input cube indexes\n");
}

#ifdef __cplusplus
extern "C" {
#endif

void EMSCRIPTEN_KEEPALIVE user_mode(int cipherId, int roundNum, int ossId, int ossParam, uint32_t cube)
{
	caw.settings(cipherId, roundNum, ossId, ossParam);
	caw.user_mode(cube);
}

#ifdef __cplusplus
}
#endif