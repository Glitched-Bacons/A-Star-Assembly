#include <vector>
#include "AStar.h"

#include <fstream>
#include <immintrin.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "Board.h"

std::vector<std::vector<int>> as::findPath(std::vector<std::vector<int>> map, Vector2<int> startingPoint,
	Vector2<int> finalPoint, Distance distanceAlgorithm)
{
	Board board(map, startingPoint, finalPoint, distanceAlgorithm);


	return board.generatePath();
}

//#define LOGGER_ENABLE

#ifdef LOGGER_ENABLE
#define logger(x) LOGGER_ENABLE logs << "[ Logger ] " << x << "\n" << printBoard(board, boardDimension.sizeX, boardDimension.sizeY) << std::endl;
#else
#define logger(x)
#endif


std::string printBoard(int* board, int width, int height)
{
	std::stringstream ss;
	int columnCounter = 0;
	for(int i = 0; i < width * height; ++i, ++columnCounter)
	{
		if (columnCounter == width)
		{
			columnCounter = 0;
			ss << "\n";
		}
		ss << board[i] << " ";
	}
	return ss.str();
}

as::Position getPosition(int index, int arrayWidth)
{
	as::Position pos;
	pos.x = index % arrayWidth;
	pos.y = index / arrayWidth;
	return pos;
}

bool isInBorder(int currentNodeIterator, int maxIterator)
{
	if (currentNodeIterator > 0 && currentNodeIterator < maxIterator)
		return true;
	return false;
}

int getIndex(int x, int y, int arrayWidth)
{
	if (x < 0 || y < 0)
		return -1;

	return x * arrayWidth + y;
}

int getEuclidianDistance(as::Position from, as::Position to)
{
	__m256d v1From[2];
	__m256d v2To[2];
	__m256d ten = _mm256_set1_pd(10);

	v1From[0] = _mm256_set1_pd(from.x);
	v1From[1] = _mm256_set1_pd(from.y);

	v2To[0] = _mm256_set1_pd(to.x);
	v2To[1] = _mm256_set1_pd(to.y);

	__m256d x_diff = _mm256_sub_pd(v1From[0], v2To[0]);
	__m256d y_diff = _mm256_sub_pd(v1From[1], v2To[1]);

	__m256d xx = _mm256_mul_pd(x_diff, x_diff);
	__m256d yy = _mm256_mul_pd(y_diff, y_diff);

	__m256d value = _mm256_add_pd(xx, yy);
	value = _mm256_sqrt_pd(value);
	value = _mm256_mul_pd(value, ten);

	double result[4];
	_mm256_store_pd(result, value);

	return static_cast<int>(result[0]);

	/*
	int x_diff = from.x - to.x;
	int y_diff = from.y - to.y;
	return static_cast<int>(std::sqrt(x_diff * x_diff + y_diff * y_diff) * 10.0);
	*/
}


as::Node* findLowestUnvisitedCost(as::Node* nodes, int pastLastElement)
{
	as::Node* lowestUnvisited = nullptr;
	for(int i = 0; i != pastLastElement; ++i)
	{
		as::Node* node = nodes + i;
		if(!node->isVisited)
		{
			if (lowestUnvisited == nullptr || node->fCost < lowestUnvisited->fCost)
				lowestUnvisited = node;
		}
	}
	return lowestUnvisited;
}

as::Node* getNode(as::Node* nodes, int& pastLastElement, const as::Position& position)
{
	for (int i = 0; i != pastLastElement; ++i)
	{
		if (nodes[i].position == position)
		{
			return nodes;
		}
	}
	return nullptr;
}

as::Node* addNode(as::Node* nodes, int& pastLastElement, const as::Position& position, 
	const as::Path& path, const int& distanceToAdd, as::Node* currentNode, 
	int* board, const as::BoardDimension& boardDimension)
{
	int distanceTillEnd = getEuclidianDistance(position, path.endPosition);
	int distanceSinceBeg = currentNode->distanceSinceBeginning + distanceToAdd;

	nodes[pastLastElement] = {
		position,
		distanceTillEnd,
		distanceSinceBeg,
		distanceTillEnd + distanceSinceBeg,
		false,
		currentNode
	};
	board[getIndex(position.x, position.y, boardDimension.sizeX)] = static_cast<int>(as::Type::Unvisited);

	as::Node* newNode = nodes + pastLastElement;
	++pastLastElement;

	return newNode;
}

void as::findPath(int* board, Node* nodes, Path path, BoardDimension boardDimension)
{
	#ifdef LOGGER_ENABLE
	std::ofstream logs("logs.txt");
	#endif

	int pastLastElement = 1;
	const int maxIterator = boardDimension.sizeX * boardDimension.sizeY;

	// ===== Add the start node ===== //
	const Position position{ path.startPosition.x, path.startPosition.y };

	nodes[0] = {
		position,		// position of the node (starting node)
		0,				// distance till the end node (rrelevant)
		0,				// distance from beginning is zero for the first node
		0,				// total F cost (irrelevant)
		false,			// it is not visited yet
		nodes			// orphan (no parent, and fatty node points to itself)
	};

	++pastLastElement;

	// ===== Proceed till all nodes are visited, or something inside while loop has returned the result ===== ///
	while(pastLastElement != maxIterator)
	{
		Node* currentNode = findLowestUnvisitedCost(nodes, pastLastElement);
		currentNode->isVisited = true;

		const int index = getIndex(currentNode->position.x, currentNode->position.y, boardDimension.sizeX);
		board[index] = static_cast<int>(Type::Visited);
		logger("current node");

		// Check if path was found
		if(currentNode->position == path.endPosition)
		{
			logger("backtracking start");
			// If so then mark final path (backtracking)
			// TODO ...
			while(currentNode->parent != currentNode)
			{
				const Position& position = currentNode->position;
				const int index = getIndex(position.x, position.y, boardDimension.sizeX);
				board[index] = static_cast<int>(Type::Path);
				currentNode = currentNode->parent;
				logger("backtracking in progress...");
			}
			logger("backtracking end");

			// Mark the final points on the map
			board[getIndex(path.startPosition.x, path.startPosition.y, boardDimension.sizeX)] = static_cast<int>(Type::StartingPoint);
			board[getIndex(path.endPosition.x, path.endPosition.y, boardDimension.sizeX)] = static_cast<int>(Type::EndingPoint);
			return;
		}

		int distances[] = {
			14, 10, 14,
			10,     10,
			14, 10, 14
		};

		Position positions[] = {
			{-1,  1}, {0,  1}, {1,  1},
			{-1,  0},               {1,  0},
			{-1, -1}, {0, -1}, {1, -1}
		};

		logger("start neigbours");
		for(int i = 0; i < 8; ++i)
		{
			const Position& posOffset = positions[i];
			Position position = {
				currentNode->position.x + posOffset.x,
				currentNode->position.y + posOffset.y
			};

			int distanceToAdd = distances[i];
			int index = getIndex(position.x, position.y, boardDimension.sizeX);

			// Check if is in border, and is not an obstacle
			if(isInBorder(index, maxIterator) && board[index] != static_cast<int>(Type::Obstacle))
			{
				// TODO: Optimize =================================================================================
				Node* node = getNode(nodes, pastLastElement, position);
				if (node == nullptr)
				{
					node = addNode(nodes, pastLastElement, position, 
						path, distanceToAdd, currentNode, board, boardDimension);
				}

				// If it was not visited yet, we may try to calculate new distance
				if(!node->isVisited)
				{
					// ... good
					int newDistance = currentNode->distanceSinceBeginning + distanceToAdd;
					if(newDistance < node->distanceSinceBeginning)
					{
						node->distanceSinceBeginning = newDistance;
						node->fCost = newDistance + node->distanceTillEnd;
						node->parent = currentNode;
					}
				}
				// skip to the next neighbor
			}
			logger("itarate...");
			// ship to the next neighbor
		}
		logger("end neighbours");

	}
}
