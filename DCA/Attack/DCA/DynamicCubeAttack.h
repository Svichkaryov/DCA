#pragma once


class DynamicCubeAttack
{
public:
	DynamicCubeAttack() = default;
	~DynamicCubeAttack() = default;

	void preprocessing_phase();
	void online_phase();

	void upper_extension(uint16_t r_u, std::initializer_list<uint16_t> I);
	void cube_tester();
	void lower_entension();

private:

};
