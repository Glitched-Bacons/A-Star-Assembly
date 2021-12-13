#pragma once
#include <string>
#include "AStar.h"

namespace as::AStarC
{
	struct BoardDimension
	{
		int sizeX;
		int sizeY;
	};

	struct Position
	{
		bool operator==(const Position& rhs) const { return x == rhs.x && y == rhs.y; }
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

	void findPath(int* board, Node* nodes, Path path, BoardDimension boardDimension);

	std::string printBoard(int* board, int width, int height);
	Position getPosition(int index, int arrayWidth);
	bool isInBorder(int currentNodeIterator, int maxIterator);
	int getIndex(int x, int y, int arrayWidth);
	int getEuclidianDistance(Position from, Position to);
	Node* findLowestUnvisitedCost(Node* nodes, int pastLastElement);
	Node* getNode(Node* nodes, int& pastLastElement, const Position& position);
	Node* addNode(Node* nodes, int& pastLastElement, const Position& position,
		const Path& path, const int& distanceToAdd, Node* currentNode,
		int* board, const BoardDimension& boardDimension);

}