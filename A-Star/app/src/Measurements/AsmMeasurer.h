#pragma once
#include <SFML/System/Vector2.hpp>

#include "PerformanceMeasurer.h"
#include "AStarCpp/AStarC.h"

namespace AsmStar
{
	struct Position
	{
		int x;
		int y;
	};

	struct Node
	{
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

	struct BoardDimension
	{
		int sizeX;
		int sizeY;
	};
}

class AsmMeasurer : public PerformanceMeasurer
{
public:
	AsmMeasurer(std::vector<int> map, const sf::Vector2<int>& startingPoint,
	            const sf::Vector2<int>& endingPoint, as::AStarC::BoardDimension boardDimensions);
	std::pair<sf::Time, AnyArray> measure() override;

private:
	std::vector<int> mMap;
	AsmStar::Path mPath;
	AsmStar::BoardDimension mBoardDimensions;
};
