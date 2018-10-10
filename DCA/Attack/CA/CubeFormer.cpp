#include "stdafx.h"
#include "CubeFormer.h"


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
		rightOne = number & -(signed)number;
		nextHigherOneBit = number + rightOne;
		rightOnesPattern = number ^ nextHigherOneBit;
		rightOnesPattern = (rightOnesPattern) / rightOne;
		rightOnesPattern >>= 2;
		next = nextHigherOneBit | rightOnesPattern;
	}

	return next;
}

