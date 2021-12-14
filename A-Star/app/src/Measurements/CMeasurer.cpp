#include "CMeasurer.h"

#include <memory>
#include <utility>

CMeasurer::CMeasurer(std::vector<int> map, sf::Vector2<int> startingPoint, sf::Vector2<int> endingPoint,
                     as::AStarC::BoardDimension boardDimensions)
	: mMap(std::move(map))
	, mPath{ {startingPoint.x, startingPoint.y}, {endingPoint.x, endingPoint.y} }
	, mBoardDimensions(boardDimensions)
{
}

std::pair<sf::Time, AnyArray> CMeasurer::measure()
{

	clock.restart();
	std::unique_ptr<as::AStarC::Node[]> nodes(new as::AStarC::Node[mMap.size()]);
	as::AStarC::findPath(mMap.data(), nodes.get(), mPath, mBoardDimensions);

	sf::Time timeElapsed = clock.getElapsedTime();
	return std::make_pair(timeElapsed, AnyArray(mMap, mBoardDimensions.sizeX));
}
