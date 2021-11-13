#include "Application.h"
#include "DllLoader.h"

#include <iostream>
#include <memory>

#include "AStarCpp/AStar.h"

int main()
{
	auto testAsmFunction = loadDllFunction<int()>(asmDll, "TestFunction");

    auto app = std::make_unique<Application>();
    app->run();

    return 0;
}
