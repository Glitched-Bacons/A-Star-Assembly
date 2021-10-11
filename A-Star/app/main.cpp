#include "Application.h"
#include "DllLoader.h"

#include <iostream>
#include <memory>

#include "AStarCpp/AStar.h"

int main()
{
	auto testAsmFunction = loadDllFunction<int()>(asmDll, "TestFunction");
	std::cout << testAsmFunction() << std::endl;

    auto testCppFunction = as::findPath(std::vector<std::vector<int>>(), as::Vector2i(0, 0), as::Vector2i(0, 0));

    auto app = std::make_unique<Application>();
    app->run();

    return 0;
}
