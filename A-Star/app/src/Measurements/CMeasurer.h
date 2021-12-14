#pragma once
#include "PerformanceMeasurer.h"
#include <AStarCpp/AStarC.h>
#include <SFML/System/Vector2.hpp>

class CMeasurer : public PerformanceMeasurer
{
public:
	CMeasurer(std::vector<int> map, sf::Vector2<int> startingPoint, 
		sf::Vector2<int> endingPoint, as::AStarC::BoardDimension boardDimensions);
	std::pair<sf::Time, AnyArray> measure() override;

private:
	std::vector<int> mMap;
	as::AStarC::Path mPath;
	as::AStarC::BoardDimension mBoardDimensions;
};
