#include "stdafx.h"
#include "CubeFormer.h"
#include "random"


uint32_t CubeFormer::get_start_cube(int dimension)
{
	return start_cubes[dimension];
}

uint32_t CubeFormer::next_cube(uint32_t number)
{
	uint32_t rightOne;
	uint32_t nextHigherOneBit;
	uint32_t rightOnesPattern;
	uint32_t next = 0;

	if (number)
	{
		rightOne         = number & -(signed)number;
		nextHigherOneBit = number + rightOne;
		rightOnesPattern = number ^ nextHigherOneBit;
		rightOnesPattern = (rightOnesPattern) / rightOne;
		rightOnesPattern >>= 2;
		next             = nextHigherOneBit | rightOnesPattern;
	}

	return next;
}

int CubeFormer::get_end_flag(int dimension)
{
	return end_flag[dimension];
}

uint32_t CubeFormer::rand_cube()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<uint32_t> dis(0x0, 0xFFFFFFFF);

	return dis(gen);
}

