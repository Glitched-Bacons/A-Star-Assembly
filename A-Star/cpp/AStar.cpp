#include <array>
#include <unordered_map>

#include "Structures/Node.h"
#include "AStar.h"

std::vector<as::Vector2<int>> as::findPath(std::array<std::array<int, 16>, 16> map, Vector2<int> startingPoint,
                                           Vector2<int> finalPoint)
{
	std::unordered_map<int, Node<as::Vector2<int>>> toBeEvaluated;
	std::unordered_map<int, Node<as::Vector2<int>>> alreadyEvaluated;




	return std::vector<as::Vector2<int>>();
}
