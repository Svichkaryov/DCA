#pragma once

enum class CubeSearchStrategy { Iterate, ByDimension, Random };

class CubeFormer
{
public:
	CubeFormer() = default;
	~CubeFormer() = default;

	uint32_t get_start_cube(int dimension);
	uint32_t next_cube(uint32_t number);
	int get_end_flag(int dimension);
	uint32_t rand_cube();

private:
	
	uint32_t start_cubes[32] = { 
		0, 1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383, 32767,
		65535, 131071, 262143, 524287, 1048575, 2097151, 4194303, 8388607, 16777215,
		33554431, 67108863, 134217727, 268435455, 536870911, 1073741823, 2147483647
	};

	int end_flag[32] = {
		0, 32, 496, 4960, 35960, 201376, 906142, 3365856, 10518300, 28048800,
		64512240, 129024480, 225792840, 347373600, 471435600, 565722720, 601080390,
		565722720, 471435600, 347373600, 225792840, 129024480, 64512240, 28048800,
		10518300, 3365856, 906142, 201376, 35960, 4960, 496, 32
	};

};
