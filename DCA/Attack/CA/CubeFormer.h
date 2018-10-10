#pragma once


class CubeFormer
{
public:
	CubeFormer() = default;
	~CubeFormer() = default;

	uint32_t get_start_cube(int dimension);
	uint32_t next_cube(uint32_t number);

private:
	uint32_t start_cubes[32] = { 
		0, 1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383, 32767,
		65535, 131071, 262143, 524287, 1048575, 2097151, 4194303, 8388607, 16777215,
		33554431, 67108863, 134217727, 268435455, 536870911, 1073741823, 2147483647
	};

};
