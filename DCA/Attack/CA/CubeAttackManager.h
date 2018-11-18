#pragma once
#include <thread>


class CubeAttackManager
{
public:
	CubeAttackManager() = default;
	~CubeAttackManager() = default;

	void cube_attack_run(Cipher_32_64* cipher);
	
	template<typename T, size_t N>
	void attack(T(&ciphers_array)[N]);

private:

};


template<typename T, size_t N>
inline void CubeAttackManager::attack(T(&ciphers_array)[N])
{
	std::vector<std::thread> threads;

	for (auto& cipher : ciphers_array)
	{
		threads.push_back(std::thread([&cipher, this] { cube_attack_run(cipher); }));
	}

	for (auto& t : threads)
	{
		t.join();
	}
}