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

	enum class Distance
	{
		Euclidean,
		Manhattan
	};


	std::vector<std::vector<int>> findPath(std::vector<std::vector<int>> map, Vector2<int> startingPoint, 
		Vector2<int> finalPoint, Distance distanceAlgorithm = Distance::Manhattan);
}
