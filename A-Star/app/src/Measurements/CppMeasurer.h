#pragma once
#include <AStarCpp/AStarModernCpp.h>
#include <SFML/System/Vector2.hpp>

#include "PerformanceMeasurer.h"


class CppMeasurer : public PerformanceMeasurer
{
public:
	CppMeasurer(std::vector<std::vector<int>> map, sf::Vector2<int> startingPoint,
	            sf::Vector2<int> finalPoint, const as::AstarModernCpp::Distance& distanceAlgorithm);
	std::pair<sf::Time, AnyArray> measure() override;

private:
	std::vector<std::vector<int>> mMap;
	as::Vector2<int> mStartingPoint;
	as::Vector2<int> mFinalPoint;
	as::AstarModernCpp::Distance mDistanceAlgorithm;
};
