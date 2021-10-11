#pragma once
#include <vector>
#include "Structures/Vector2.h"


namespace as
{
	std::vector<as::Vector2<int>> findPath(std::vector<std::vector<int>> map, Vector2<int> startingPoint, Vector2<int> finalPoint);
	std::vector<std::vector<int>> markPath(std::vector<std::vector<int>> map, Vector2<int> startingPoint, Vector2<int> finalPoint);


}
