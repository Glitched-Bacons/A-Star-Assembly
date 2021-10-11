#pragma once
#include <vector>
#include "Structures/Vector2.h"

namespace as
{
	std::vector<Vector2<int>> findPath(std::array<std::array<int, 16>, 16> map, Vector2<int> startingPoint, Vector2<int> finalPoint);


}
