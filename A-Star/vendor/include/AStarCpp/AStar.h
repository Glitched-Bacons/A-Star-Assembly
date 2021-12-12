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

	struct BoardDimension
	{
		int sizeX;
		int sizeY;
	};

	struct Position
	{
		bool operator==(const Position& rhs) const { return x == rhs.x && y == rhs.y;  }
		int x;
		int y;
	};

	struct Node
	{
		Node() = default;

		Position position;
		int distanceTillEnd;
		int distanceSinceBeginning;
		int fCost;
		bool isVisited;
		Node* parent;
	};

	struct Path
	{
		Position startPosition;
		Position endPosition;
	};


	std::vector<std::vector<int>> findPath(std::vector<std::vector<int>> map, Vector2<int> startingPoint, 
		Vector2<int> finalPoint, Distance distanceAlgorithm = Distance::Manhattan);

	void findPath(int* board, Node* nodes, Path path, BoardDimension boardDimension);
}
