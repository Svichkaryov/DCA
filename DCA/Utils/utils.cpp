#include "stdafx.h"
#include <chrono>
#include <Windows.h>


namespace utils
{
	template<class Function, typename funcNameType, typename... Args>
	auto executeAndGetElapsedTime(Function&& f, funcNameType funcName, Args&&... args)
		-> typename std::enable_if<
		!std::is_same<decltype(f(std::forward<Args>(args)...)), void>::value,
		decltype(f(std::forward<Args>(args)...))>::type
	{
		auto t1 = std::chrono::high_resolution_clock::now();
		auto res = f(std::forward<Args>(args)...);
		auto t2 = std::chrono::high_resolution_clock::now();

		auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		auto hours = elapsed_time / 3600000;
		auto minutes = elapsed_time / 60000;
		auto seconds = elapsed_time / 1000;
		auto millisecons = elapsed_time % 1000;
		printf("Elapsed time of %s ( %s ) : %d::%d::%d::%d\n", funcName, typeid(f).name(),
			hours, minutes, seconds, millisecons);

		return res;
	}

	template<class Function, typename funcNameType, typename... Args>
	auto executeAndGetElapsedTime(Function&& f, funcNameType funcName, Args&&... args)
		-> typename std::enable_if<
		std::is_same<decltype(f(std::forward<Args>(args)...)), void>::value,
		std::string>::type
	{
		auto t1 = std::chrono::high_resolution_clock::now();
		f(std::forward<Args>(args)...);
		auto t2 = std::chrono::high_resolution_clock::now();

		auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		auto hours = elapsed_time / 3600000;
		auto minutes = elapsed_time / 60000;
		auto seconds = elapsed_time / 1000;
		auto millisecons = elapsed_time % 1000;
			printf("Elapsed time of %s ( %s ) : %d::%d::%d::%d\n", funcName, typeid(f).name(),
				hours, minutes, seconds, millisecons);

		return "Nothing to return (returning type - void)";
	}

}