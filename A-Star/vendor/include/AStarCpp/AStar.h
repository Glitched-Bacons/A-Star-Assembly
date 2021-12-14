#pragma once
#include <vector>
#include "Structures/Vector2.h"

namespace as
{
	enum class Type : uint8_t
	{
		Nothing = 0,
		Obstacle = 1,
		Visited = 2,
		Unvisited = 3,
		Path = 4,
		StartingPoint = 5,
		EndingPoint = 6
	};
}