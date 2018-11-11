#pragma once
#include <thread>


class CubeAttackManager
{
public:
	CubeAttackManager() = default;
	~CubeAttackManager() = default;

	void threadFunc(CipherARX_32_64* cipher);
	
	template<typename T, size_t N>
	void attack(T(&a)[N]);

private:

};


template<typename T, size_t N>
inline void CubeAttackManager::attack(T(&a)[N])
{
	std::vector<std::thread> threads;

	for (auto& el : a)
	{
		threads.push_back(std::thread([&el, this] { threadFunc(el); }));
	}

	for (auto& t : threads)
	{
		t.join();
	}
}
