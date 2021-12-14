#include "AStarModernCpp.h"

#include <algorithm>
#include <stdexcept>


as::AStarModernCpp::AStarModernCpp(std::vector<std::vector<int>> map, Vector2<int> startingPoint, 
	Vector2<int> finalPoint, AstarModernCpp::Distance distanceAlgorithm)
	: mStartingPoint(std::move(startingPoint))
	, mFinalPoint(std::move(finalPoint))
	, mMap(std::move(map))
	, mHeightOfMap(mMap.size())
	, mWidthOfMap(mMap[0].size())
{
	selectDistanceAlgorithm(distanceAlgorithm);
}

void as::AStarModernCpp::selectDistanceAlgorithm(as::AstarModernCpp::Distance distanceAlgorithm)
{
	switch (distanceAlgorithm)
	{
	case AstarModernCpp::Distance::Euclidean:
		mDistanceAlgorithm = [](const Vector2i& lhs, const Vector2i& rhs) { return lhs.getEuclideanDistanceTo(rhs); };
		break;
	case AstarModernCpp::Distance::Manhattan:
		mDistanceAlgorithm = [](const Vector2i& lhs, const Vector2i& rhs) { return lhs.getManhattanDistanceTo(rhs); };
		break;
	}
}

void as::AStarModernCpp::checkNodesAround(const as::Vector2<int>& node, const Cost& costOfNode)
{
	// Those are values used to calculate indexes of nodes around
	static const Vector2<int> offsetsAround[] = {
		{-1,  1}, {0,  1}, {1,  1},
		{-1,  0},          {1,  0},
		{-1, -1}, {0, -1}, {1, -1}
	};

	static const int distancesAround[] = {
		14, 10, 14,
		10,     10,
		14, 10, 14
	};

	// We have to check every node around
	for(int neighbourIndex = 0; neighbourIndex < 8; ++neighbourIndex)
	{
		if(const auto closeNode = node + offsetsAround[neighbourIndex]; isInBorders(closeNode) && getTypeFromMap(closeNode) != Type::Obstacle)
		{
			if(closeNode == mFinalPoint)
			{
				isFinalPointFound = true;
				return;
			}

			const auto distanceBetweenCloseNode = distancesAround[neighbourIndex];

			auto costOfCloseNode = Cost(costOfNode.distanceFromStart() 
				+ distanceBetweenCloseNode, mDistanceAlgorithm(closeNode, mFinalPoint) * 10, node);

			if (auto unvisitedNode = mVisitedNodes.find(closeNode); unvisitedNode == mVisitedNodes.end())
			{
				setNodeTypeOnMap(closeNode, Type::Unvisited);
				mUnvisitedNodes.emplace(std::move(costOfCloseNode), closeNode);
			}
				
		}
	}
}

void as::AStarModernCpp::drawFinalPathOnMap(const Vector2<int>& lastNode)
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

void as::AStarModernCpp::updateMapAroundNode(const Vector2<int>& node, const Cost& costOfNode)
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

bool as::AStarModernCpp::isInBorders(const Vector2<int>& node) const
{
	return node.x >= 0 && node.y >= 0 &&
		node.x < mWidthOfMap && node.y < mHeightOfMap;
}

std::vector<std::vector<int>> as::AStarModernCpp::generatePath()
{
	updateMapAroundNode(mStartingPoint, 
		Cost{ 0,
			mDistanceAlgorithm(mStartingPoint, mFinalPoint),
			mStartingPoint
		}
	);

	// Always take from the front - it means the cheaper node as it is sorted by cost
	for(auto it = mUnvisitedNodes.begin(); it != mUnvisitedNodes.end(); it = mUnvisitedNodes.begin())
	{
		const auto& [cost, node] = *it;

		// There was already cheaper founding before and it was visited
		if(mVisitedNodes.find(node) != mVisitedNodes.end())
		{
			mUnvisitedNodes.erase(it);
			continue;
		}

		setNodeTypeOnMap(node, Type::Visited);
		updateMapAroundNode(node, cost);

		if (isFinalPointFound)
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

as::AStarModernCpp::Cost::Cost(int distanceFromStart, int distanceTillEnd, Vector2<int> cameFrom)
	: mDistanceFromStart(distanceFromStart)
	, mDistanceTillEnd(distanceTillEnd)
	, mCameFrom(std::move(cameFrom))
	, mTotalCost(distanceFromStart + distanceTillEnd)
{
			
}

as::AStarModernCpp::Cost::Cost(const Cost&) = default;
as::AStarModernCpp::Cost::Cost(Cost&&) noexcept = default;
as::AStarModernCpp::Cost& as::AStarModernCpp::Cost::operator=(Cost&& rhs) noexcept = default;

int as::AStarModernCpp::Cost::distanceFromStart() const
{
	return mDistanceFromStart;
}

int as::AStarModernCpp::Cost::distanceTillEnd() const
{
	return mDistanceTillEnd;
}

int as::AStarModernCpp::Cost::totalCost() const
{
	return mTotalCost;
}

as::Vector2<int> as::AStarModernCpp::Cost::cameFrom() const
{
	return mCameFrom;
}

bool as::AStarModernCpp::Cost::operator<(const Cost& rhs) const
{
	return totalCost() < rhs.totalCost();
}

void as::AStarModernCpp::setNodeTypeOnMap(const Vector2<int>& node, Type type)
{
	mMap[node.x][node.y] = static_cast<int>(type);
}

as::Type as::AStarModernCpp::getTypeFromMap(const Vector2<int>& node)
{
	return static_cast<Type>(mMap[node.x][node.y]);
}

const char* as::AstarModernCpp::toString(const Distance& distance)
{
	switch (distance)
	{
	case Distance::Euclidean: return "Euclidean";
	case Distance::Manhattan: return "Manhattan";
	default: return "No default";
	}
}
