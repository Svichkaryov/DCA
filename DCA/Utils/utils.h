#pragma once


namespace utils
{
	template<class Function, typename funcNameType, typename... Args>
	auto executeAndGetElapsedTime(Function&& f, funcNameType funcName, Args&&... args)
		-> typename std::enable_if<
		!std::is_same<decltype(f(std::forward<Args>(args)...)), void>::value,
		decltype(f(std::forward<Args>(args)...))>::type;

	template<class Function, typename funcNameType, typename... Args>
	auto executeAndGetElapsedTime(Function&& f, funcNameType funcName, Args&&... args)
		-> typename std::enable_if<
		std::is_same<decltype(f(std::forward<Args>(args)...)), void>::value,
		std::string>::type;
}

#include "utils.cpp"