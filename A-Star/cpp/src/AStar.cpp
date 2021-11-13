#include <vector>
#include "AStar.h"
#include "Board.h"

std::vector<std::vector<int>> as::findPath(std::vector<std::vector<int>> map, Vector2<int> startingPoint,
	Vector2<int> finalPoint, Distance distanceAlgorithm)
{
	Board board(map, startingPoint, finalPoint, distanceAlgorithm);


	return board.generatePath();
}
