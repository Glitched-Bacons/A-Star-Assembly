#pragma once
#include <functional>
#include <map>

#include "AStar.h"
#include "Structures/Vector2.h"

namespace as
{

class Board
{
public:
	Board(std::vector<std::vector<int>> map, Vector2<int> startingPoint, Vector2<int> finalPoint, Distance distanceAlgorithm);
	std::vector<std::vector<int>> generatePath();

private:

	struct Cost
	{

		Cost(int distanceFromStart, float distanceTillEnd, Vector2<int> cameFrom);

		Cost(const Cost&);
		Cost(Cost&&) noexcept;
		Cost& operator=(Cost&& rhs) noexcept;

		[[nodiscard]] int distanceFromStart() const;
		[[nodiscard]] int distanceTillEnd() const;
		[[nodiscard]] int totalCost() const;
		[[nodiscard]] Vector2<int> cameFrom() const;
		bool operator<(const Cost& rhs) const;

	private:
		int mDistanceFromStart;
		float mDistanceTillEnd;
		Vector2<int> mCameFrom;
		int mTotalCost;
	};

private:
	void updateMapAroundNode(const Vector2<int>& node, const Cost& costOfNode);
	[[nodiscard]] bool isInBorders(const Vector2<int>& node) const;
	void checkNodesAround(const as::Vector2<int>& node, const ::as::Board::Cost& costOfNode);
	void drawFinalPathOnMap(const Vector2<int>& lastNode);
	void setNodeTypeOnMap(const Vector2<int>& node, Type type);
	void selectDistanceAlgorithm(as::Distance distanceAlgorithm);
	as::Type getTypeFromMap(const Vector2<int>& node);

private:
	const Vector2<int> mStartingPoint;
	const Vector2<int> mFinalPoint;

	std::unordered_map<Vector2<int>, Cost> mVisitedNodes;
	std::multimap<Cost, Vector2<int>> mUnvisitedNodes;

	std::vector<std::vector<int>> mMap;
	std::function<float(Vector2i, Vector2i)> mDistanceAlgorithm;

	const int mHeightOfMap;
	const int mWidthOfMap;
};


}
