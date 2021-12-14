#include "AsmMeasurer.h"

#include <memory>
#include <utility>

#include "DllLoader.h"

AsmMeasurer::AsmMeasurer(std::vector<int> map, const sf::Vector2<int>& startingPoint, 
                         const sf::Vector2<int>& endingPoint, as::AStarC::BoardDimension boardDimensions)
	: mMap(std::move(map))
	, mPath{ {startingPoint.x, startingPoint.y}, {endingPoint.x, endingPoint.y} }
	, mBoardDimensions{ boardDimensions.sizeX, boardDimensions.sizeY }
{
}

std::pair<sf::Time, AnyArray> AsmMeasurer::measure()
{
	auto testAsmFunction = loadDllFunction<void(int[], AsmStar::Node*, AsmStar::Path*, AsmStar::BoardDimension*)>(asmDll, "aStarSearch");

	clock.restart();
	std::unique_ptr<AsmStar::Node[]> nodes(new AsmStar::Node[mMap.size()]);
	testAsmFunction(mMap.data(), nodes.get(), &mPath, &mBoardDimensions);

	sf::Time timeElapsed = clock.getElapsedTime();
	return std::make_pair(timeElapsed, AnyArray(mMap, mBoardDimensions.sizeX));
}
