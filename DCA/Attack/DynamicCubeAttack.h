#pragma once


class DynamicCubeAttack
{
public:
	DynamicCubeAttack() = default;
	~DynamicCubeAttack() = default;

	void preprocessing_phase();
	void online_phase();

	void upper_extension();
	void cube_tester();
	void lower_entension();

private:

};
