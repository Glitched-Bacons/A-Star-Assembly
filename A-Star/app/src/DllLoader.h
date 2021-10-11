#pragma once
#include <functional>
#include <windows.h>
#include <stdexcept>

constexpr const char* asmDll = "../vendor/lib/AStar/asm.dll";

template <typename T>
struct TypeParser {};

template <typename Ret, typename... Args>
struct TypeParser<Ret(Args...)> {
	static std::function<Ret(Args...)> createFunction(const FARPROC lpfnGetProcessID) {
		return std::function<Ret(Args...)>(reinterpret_cast<Ret(__stdcall*)(Args...)>(lpfnGetProcessID));
	}
};

template <typename CallableFunction>
std::function<CallableFunction> loadDllFunction(const std::string& pathToFile, const std::string& functionName)
{
	const HINSTANCE dll = LoadLibraryA(pathToFile.c_str());

	if (!dll)
	{
		throw std::runtime_error("Cannot open dll");
	}

	const FARPROC proc2 = GetProcAddress(dll, functionName.c_str());

	return TypeParser<CallableFunction>::createFunction(proc2);
}