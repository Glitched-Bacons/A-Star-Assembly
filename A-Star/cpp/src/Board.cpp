#include "Board.h"

#include <algorithm>
#include <stdexcept>


as::Board::Board(std::vector<std::vector<int>> map, Vector2<int> startingPoint, Vector2<int> finalPoint, Distance distanceAlgorithm)
	: mStartingPoint(std::move(startingPoint))
	, mFinalPoint(std::move(finalPoint))
	, mMap(std::move(map))
	, mHeightOfMap(mMap.size())
	, mWidthOfMap(mMap[0].size())
{
	selectDistanceAlgorithm(distanceAlgorithm);
}

void as::Board::selectDistanceAlgorithm(as::Distance distanceAlgorithm)
{
	switch (distanceAlgorithm)
	{
	case Distance::Euclidean:
		mDistanceAlgorithm = [](const Vector2i& lhs, const Vector2i& rhs) { return lhs.getEuclideanDistanceTo(rhs); };
		break;
	case Distance::Manhattan:
		mDistanceAlgorithm = [](const Vector2i& lhs, const Vector2i& rhs) { return lhs.getManhattanDistanceTo(rhs); };
		break;
	}
}

void as::Board::checkNodesAround(const as::Vector2<int>& node, const Cost& costOfNode)
{
	// Those are values used to calculate indexes of nodes around
	static const Vector2<int> offsetsAround[] = {
		{-1,  1}, {0,  1}, {1,  1},
		{-1,  0},          {1,  0},
		{-1, -1}, {0, -1}, {1, -1}
	};

	// Special case of node, where the distance is a little larger
	// Because of Pythagoras - Diagonal distance :T
	static const Vector2<int> cornerValues[] = {
		{-1,  1}, {1,  1},
		{-1, -1}, {1, -1}
	};

	// We have to check every node around
	for(auto& offset : offsetsAround)
	{
		if(const auto closeNode = node + offset; isInBorders(closeNode) && getTypeFromMap(closeNode) != Type::Obstacle)
		{
			auto distanceBetweenCloseNode = 10;
			if (std::find(std::begin(cornerValues), std::end(cornerValues), offset) != std::end(cornerValues))
			{
				distanceBetweenCloseNode = 14;
			}

			auto costOfCloseNode = Cost(costOfNode.distanceFromStart() 
				+ distanceBetweenCloseNode, mDistanceAlgorithm(closeNode, mFinalPoint), node);

			if (auto cheaperNode = mVisitedNodes.find(closeNode); cheaperNode == mVisitedNodes.end() 
				|| cheaperNode->second.totalCost() > costOfCloseNode.totalCost())
			{
				setNodeTypeOnMap(closeNode, Type::Unvisited);
				mUnvisitedNodes.emplace(std::move(costOfCloseNode), closeNode);
			}
				
		}
	}
}

void as::Board::drawFinalPathOnMap(const Vector2<int>& lastNode)
{
	setNodeTypeOnMap(lastNode, Type::Path);
	// Go back till we find the first node ever
	// The first node ever has it predecessor mark as itself
	for (auto nodeItCameFrom = mVisitedNodes.at(lastNode).cameFrom();
		nodeItCameFrom != mVisitedNodes.at(nodeItCameFrom).cameFrom();
		nodeItCameFrom = mVisitedNodes.at(nodeItCameFrom).cameFrom())
	{
		setNodeTypeOnMap(nodeItCameFrom, Type::Path);
	}
	setNodeTypeOnMap(mStartingPoint, Type::Path);
}

void as::Board::updateMapAroundNode(const Vector2<int>& node, const Cost& costOfNode)
{
	if(isInBorders(node))
	{
		checkNodesAround(node, costOfNode);
		setNodeTypeOnMap(node, Type::Visited);
		mVisitedNodes.emplace(node, costOfNode);
	}
	else
	{
		throw std::runtime_error("Out of border");
	}
}

bool as::Board::isInBorders(const Vector2<int>& node) const
{
	return node.x >= 0 && node.y >= 0 &&
		node.x < mWidthOfMap && node.y < mHeightOfMap;
}

std::vector<std::vector<int>> as::Board::generatePath()
{
	updateMapAroundNode(mStartingPoint, Cost{ 0, mDistanceAlgorithm(mStartingPoint, mFinalPoint), mStartingPoint });

	// Always take from the front - it means the cheaper node as it is sorted by cost
	for(auto it = mUnvisitedNodes.begin(); it != mUnvisitedNodes.end(); it = mUnvisitedNodes.begin())
	{
		const auto& [cost, node] = *it;

		setNodeTypeOnMap(node, Type::Visited);
		updateMapAroundNode(node, cost);

		if (const auto& [cost, node] = *it; cost.distanceTillEnd() <= 15)
		{
			drawFinalPathOnMap(node);
			break;
		}
		mUnvisitedNodes.erase(it);
	}
	setNodeTypeOnMap(mStartingPoint, Type::StartingPoint);
	setNodeTypeOnMap(mFinalPoint, Type::EndingPoint);

	return mMap;
}

as::Board::Cost::Cost(int distanceFromStart, float distanceTillEnd, Vector2<int> cameFrom): mDistanceFromStart(distanceFromStart)
	, mDistanceTillEnd(distanceTillEnd * 10)
	, mCameFrom(std::move(cameFrom))
	, mTotalCost(distanceFromStart + distanceTillEnd * 10)
{
			
}

as::Board::Cost::Cost(const Cost&) = default;
as::Board::Cost::Cost(Cost&&) noexcept = default;
as::Board::Cost& as::Board::Cost::operator=(Cost&& rhs) noexcept = default;

int as::Board::Cost::distanceFromStart() const
{
	return mDistanceFromStart;
}

int as::Board::Cost::distanceTillEnd() const
{
	return mDistanceTillEnd;
}

int as::Board::Cost::totalCost() const
{
	return mTotalCost;
}

as::Vector2<int> as::Board::Cost::cameFrom() const
{
	return mCameFrom;
}

bool as::Board::Cost::operator<(const Cost& rhs) const
{
	return totalCost() < rhs.totalCost();
}

void as::Board::setNodeTypeOnMap(const Vector2<int>& node, Type type)
{
	mMap[node.x][node.y] = static_cast<int>(type);
}

as::Type as::Board::getTypeFromMap(const Vector2<int>& node)
{
	return static_cast<Type>(mMap[node.x][node.y]);
}
