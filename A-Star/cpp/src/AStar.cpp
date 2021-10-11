#include <vector>
#include <unordered_map>

#include "Structures/Node.h"
#include "AStar.h"

std::vector<as::Vector2<int>> as::findPath(std::vector<std::vector<int>> map, Vector2<int> startingPoint,
                                           Vector2<int> finalPoint)
{
	std::unordered_map<int, Node<as::Vector2<int>>> toBeEvaluated;
	std::unordered_map<int, Node<as::Vector2<int>>> alreadyEvaluated;




	return std::vector<as::Vector2<int>>();
}

std::vector<std::vector<int>> as::markPath(std::vector<std::vector<int>> map, Vector2<int> startingPoint,
	Vector2<int> finalPoint)
{
	return std::vector<std::vector<int>>();
}
