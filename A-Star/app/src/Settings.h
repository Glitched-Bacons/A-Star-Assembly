#pragma once


enum class Algorithm
{
	Cpp,
	C,
	Asm,

	COUNTER
};

static inline const char* toString(const Algorithm& algorithm)
{
	switch (algorithm)
	{
	case Algorithm::Cpp: return "C++";
	case Algorithm::C: return "C/C++03";
	case Algorithm::Asm: return "Assembler";
	default: return "Not found";
	}
}