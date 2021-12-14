#include "Board.h"

#include <iostream>
#include <numeric>
#include <imgui/imgui.h>
#include <implot/implot.h>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

#include "DllLoader.h"
#include "Settings.h"
#include "AStarCpp/AStar.h"
#include "AStarCpp/AStarModernCpp.h"
#include "AStarCpp/AStarC.h"
#include "ImguiDisplay/StatisticDisplayer.h"
#include "Measurements/AsmMeasurer.h"
#include "Measurements/CMeasurer.h"
#include "Measurements/CppMeasurer.h"



Board::Board(int width, int height, float tileSize)
	: mTileSize(tileSize)
	, mWidth(width)
	, mHeight(height)
	, mShouldObstaclesBeRemoved(false)
	, mCurrentlyShownAlgorithm(Algorithm::Cpp)
	, mDistanceAlgorithm(as::AstarModernCpp::Distance::Euclidean)
{
	mBoard.resize(mHeight);
	for (auto& row : mBoard)
	{
		row.resize(mWidth);
	}

	for (int row = 0; row < mHeight; ++row)
	{
		for (int column = 0; column < mWidth; ++column)
		{
			auto tile = std::make_unique<Tile>(Tile::Type::Nothing, tileSize);;
			tile->setPosition(sf::Vector2f(row * tileSize, column * tileSize));

			mBoard[row][column] = std::move(tile);
		}
	}
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& row : mBoard)
	{
		for (auto& column : row)
		{
			column->draw(target, states);
		}
	}
}

void Board::refreshAStarAlgorithm()
{
	auto startingPoint = getStartingPoint();
	auto endingPoint = getEndingPoint();
	if (startingPoint.has_value() && endingPoint.has_value())
	{
		std::map<Algorithm, std::unique_ptr<PerformanceMeasurer>> algorithms;

		as::AStarC::BoardDimension boardDimension{ mBoard[0].size(), mBoard.size() };
		auto startingPoint = getStartingPoint().value();
		auto endingPoint = getEndingPoint().value();

		algorithms.emplace(Algorithm::Asm, 
			std::make_unique<AsmMeasurer>(getArray1D(), startingPoint, endingPoint, boardDimension));
		algorithms.emplace(Algorithm::C, 
			std::make_unique<CMeasurer>(getArray1D(), startingPoint, endingPoint, boardDimension));
		algorithms.emplace(Algorithm::Cpp, 
			std::make_unique<CppMeasurer>(getArray2D(), startingPoint, endingPoint, mDistanceAlgorithm));

		for(const auto& [algorithm, measurer] : algorithms)
		{
			const auto& [time, resultArray] = measurer->measure();
			mAlgorithmsExecutions.emplace(algorithm, time);
			mLastAlgorithmExecutions[algorithm] = resultArray;
		}

		setBoard(mLastAlgorithmExecutions[mCurrentlyShownAlgorithm]);
	}
}

bool Board::isPositionInBoard(const sf::Vector2i& clickedPosition)
{
	if (clickedPosition.x < 0 || clickedPosition.y < 0 ||
		clickedPosition.x >= mWidth * mTileSize || clickedPosition.y >= mHeight * mTileSize)
		return false;
	return true;
}

void Board::processEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		// ignore clicks outside of board
		auto clickedPosition = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
		if (!isPositionInBoard(clickedPosition)) 
			return;

		auto& selectedTile = getTile(clickedPosition);

		if (event.mouseButton.button == sf::Mouse::Left)
		{
			if (mStartingPoint.has_value())
			{
				getTile(mStartingPoint.value()).setTileType(Tile::Type::Nothing);
			}
			selectedTile.setTileType(Tile::Type::StartingPoint);
			mStartingPoint = clickedPosition;
		}
		else if (event.mouseButton.button == sf::Mouse::Right)
		{
			if (mEndingPoint.has_value())
			{
				getTile(mEndingPoint.value()).setTileType(Tile::Type::Nothing);
			}
			selectedTile.setTileType(Tile::Type::EndingPoint);
			mEndingPoint = clickedPosition;
		}
		else if (event.mouseButton.button == sf::Mouse::Middle)
		{
			if (selectedTile.getTileType() == Tile::Type::Obstacle)
				mShouldObstaclesBeRemoved = true;
			else
				mShouldObstaclesBeRemoved = false;
		}
	}

	if (event.type == sf::Event::MouseButtonReleased)
	{
		refreshAStarAlgorithm();
	}
}

void Board::fixedUpdate(float deltaTime)
{

}

void Board::updateMouse(const sf::Vector2i& mousePosition)
{
	if(sf::Mouse::isButtonPressed(sf::Mouse::Middle))
	{
		auto& selectedTile = getTile(mousePosition);
		if (mShouldObstaclesBeRemoved)
		{
			if (selectedTile.getTileType() == Tile::Type::Obstacle)
			{
				selectedTile.setTileType(Tile::Type::Nothing);
			}
		}
		else
			selectedTile.setTileType(Tile::Type::Obstacle);
	}
}

void Board::setBoard(Array1D arrayBoard)
{
	for (int row = 0; row < mHeight; ++row)
	{
		for (int column = 0; column < mWidth; ++column)
		{
			auto& tileNumber = arrayBoard[row * mWidth + column];
			mBoard[row][column]->setTileType(static_cast<Tile::Type>(tileNumber));
		}
	}
}

void Board::setBoard(Array2D arrayBoard)
{
	for (int row = 0; row < mHeight; ++row)
	{
		for (int column = 0; column < mWidth; ++column)
		{
			auto& tileNumber = arrayBoard[row][column];
			mBoard[row][column]->setTileType(static_cast<Tile::Type>(tileNumber));
		}
	}
}

typename Board::Array1D Board::getArray1D() const
{
	Array1D arrayBoard(mWidth * mHeight);

	for (int row = 0; row < mHeight; ++row)
	{
		for (int column = 0; column < mWidth; ++column)
		{
			auto tileType = mBoard[row][column]->getTileType();
			switch (tileType)
			{
			case Tile::Type::Obstacle:
				arrayBoard[row * mWidth + column] = static_cast<int>(tileType);
				break;

			default:
				arrayBoard[row * mWidth + column] = static_cast<int>(Tile::Type::Nothing);
			}
		}
	}

	return arrayBoard;
}

typename Board::Array2D Board::getArray2D() const
{
	Array2D arrayBoard;
	arrayBoard.resize(mHeight);
	for (auto& row : arrayBoard)
		row.resize(mWidth);

	for (int row = 0; row < mHeight; ++row)
	{
		for (int column = 0; column < mWidth; ++column)
		{
			auto tileType = mBoard[row][column]->getTileType();
			switch(tileType)
			{
			case Tile::Type::Obstacle:
				arrayBoard[row][column] = static_cast<int>(tileType);
				break;

			default:
				arrayBoard[row][column] = static_cast<int>(Tile::Type::Nothing);
			}
		}
	}

	return arrayBoard;
}

std::optional<sf::Vector2i> Board::getStartingPoint() const
{
	if (mStartingPoint.has_value())
	{
		int tileIndexX = mStartingPoint.value().x / mTileSize;
		int tileIndexY = mStartingPoint.value().y / mTileSize;

		return sf::Vector2i(tileIndexX, tileIndexY);
	}
	return std::nullopt;
}

std::optional<sf::Vector2i> Board::getEndingPoint() const
{
	if (mEndingPoint.has_value())
	{
		int tileIndexX = mEndingPoint.value().x / mTileSize;
		int tileIndexY = mEndingPoint.value().y / mTileSize;

		return sf::Vector2i(tileIndexX, tileIndexY);
	}
	return std::nullopt;
}

void Board::refreshAlgorithmDisplay()
{
	if (!mLastAlgorithmExecutions.empty())
	{
		setBoard(mLastAlgorithmExecutions[mCurrentlyShownAlgorithm]);
	}
}

std::vector<AlgorithmMeasurments> Board::getMeasurements()
{
	std::vector<AlgorithmMeasurments> measurments;
	for (auto beg = mAlgorithmsExecutions.begin(), end = mAlgorithmsExecutions.end();
	     beg != end; beg = mAlgorithmsExecutions.upper_bound(beg->first))
	{
		AlgorithmMeasurments measurment;
		const auto& algorithm = beg->first;
		measurment.algorithmName = toString(algorithm);

		const auto noOfMeasurments = static_cast<float>(mAlgorithmsExecutions.count(algorithm));

		measurment.numberOfValuesToDisplay = (noOfMeasurments < maxMeasurmentsDisplay) ? noOfMeasurments : maxMeasurmentsDisplay;
		measurment.dataOffset = (noOfMeasurments < maxMeasurmentsDisplay) ? 0 : noOfMeasurments - maxMeasurmentsDisplay;

		auto timeElapsed = std::vector<float>();
		std::transform(beg, mAlgorithmsExecutions.upper_bound(beg->first), std::back_inserter(timeElapsed),
		               [](const auto& iterator)
		               {
			               return iterator.second.asMicroseconds() / 1000.0;
		               });
		measurment.timeElapsed = timeElapsed;
		measurments.push_back(measurment);
	}
	return measurments;
}

void Board::updateImGui()
{
	ImGui::Begin("Measurments", 0, ImGuiWindowFlags_NoMove);
	statisticDisplayer.imGuiDisplayLastMeasurements(getMeasurements());
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	auto beforeUpdateAlgorithm = mCurrentlyShownAlgorithm;
	statisticDisplayer.imGuiUpdateSelectedAlgorithm(mCurrentlyShownAlgorithm);
	if (beforeUpdateAlgorithm != mCurrentlyShownAlgorithm) refreshAlgorithmDisplay();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::TextWrapped("C++ algorithm additional settings");

	auto beforeUpdateDistance = mDistanceAlgorithm;
	statisticDisplayer.imGuiUpdateSelectedDistanceFunctionCpp(mDistanceAlgorithm);
	if (beforeUpdateDistance != mDistanceAlgorithm) refreshAStarAlgorithm();

	ImGui::End();
}

Tile& Board::getTile(const sf::Vector2i& mousePosition)
{
	int tileIndexX = mousePosition.x / mTileSize;
	int tileIndexY = mousePosition.y / mTileSize;

	return *mBoard[tileIndexX][tileIndexY];
}
