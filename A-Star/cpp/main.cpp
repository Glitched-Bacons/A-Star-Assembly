#include <iostream>
#include <windows.h>
#include <stdexcept>

#include "Structures/Node.h"
#include "Structures/Vector2.h"

int main()
{
	forCppTestingPurposes();
	forAsmTestingPurposes();
	return 0;
}

void forAsmTestingPurposes()
{
	std::cout << "ASM TESTING =====";
	using MyProc1 = int(__stdcall*) (DWORD x);

	int x = 3, y = 4, z = 0;
	const HINSTANCE dll = LoadLibrary(TEXT("asm.dll"));

	if (!dll)
	{
		throw std::runtime_error("Cannot open dll");
	}

	const FARPROC proc2 = GetProcAddress(dll, "MyProc1");

	MyProc1 callableFuntion = reinterpret_cast<MyProc1>(proc2);
	z = callableFuntion(x);

	std::cout << z;
}

void forCppTestingPurposes()
{
	std::cout << "CPP TESTING =====";
	as::Node<as::Vector2i> node1({ 4, 2 });
	as::Node<as::Vector2i> node2({ 2, 4 });

	std::cout << node1.getCostTo(node2) << std::endl;
}
