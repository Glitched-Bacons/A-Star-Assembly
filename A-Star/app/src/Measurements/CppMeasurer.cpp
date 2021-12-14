#include "CppMeasurer.h"

#include <utility>

CppMeasurer::CppMeasurer(std::vector<std::vector<int>> map, sf::Vector2<int> startingPoint,
                         sf::Vector2<int> finalPoint, const as::AstarModernCpp::Distance& distanceAlgorithm)
	: mMap(std::move(map))
	, mStartingPoint(std::move(startingPoint.x), std::move(startingPoint.y))
	, mFinalPoint( std::move(finalPoint.x), std::move(finalPoint.y) )
	, mDistanceAlgorithm(distanceAlgorithm)
{
}

std::pair<sf::Time, AnyArray> CppMeasurer::measure()
{
	clock.restart();
	as::AStarModernCpp modernCpp(mMap, mStartingPoint, mFinalPoint, mDistanceAlgorithm);
	auto foundPath = AnyArray(modernCpp.generatePath());

	sf::Time timeResult = clock.getElapsedTime();
	return std::make_pair(timeResult, foundPath);
}
