#include "Board.h"

#include <numeric>
#include <imgui/imgui.h>
#include <implot/implot.h>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include "AStarCpp/AStar.h"


Board::Board(int width, int height, float tileSize)
	: mTileSize(tileSize)
	, mWidth(width)
	, mHeight(height)
	, mCurrentlyShownAlgorithm(Algorithm::Cpp)
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
		auto beginning = as::Vector2<int>(std::move(startingPoint.value().x), std::move(startingPoint.value().y));
		auto ending = as::Vector2<int>(std::move(endingPoint.value().x), std::move(endingPoint.value().y));

		sf::Clock clock;
		auto foundPathCpp= as::findPath(getArray2D(), beginning, ending);
		mTimeMeasurementsOfCpp.emplace_back(clock.getElapsedTime().asMicroseconds() / 1000.f);

		clock.restart();
		auto foundPathAsm = as::findPath(getArray2D(), beginning, ending);
		mTimeMeasurementsOfAsm.emplace_back(clock.getElapsedTime().asMicroseconds() / 1000.f);

		switch (mCurrentlyShownAlgorithm)
		{
		case Algorithm::Cpp:
			setBoard(foundPathCpp);
			break;
		case Algorithm::Asm:
			setBoard(foundPathAsm);
			break;
		}
		mLastAsmExecution.emplace(std::move(foundPathAsm));
		mLastCppExecution.emplace(std::move(foundPathCpp));
	}
}

void Board::processEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		auto clickedPosition = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
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

void Board::updateMouse(sf::Vector2i mousePosition)
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
			auto& tileNumber = arrayBoard[row * 10 + column];
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
	Array1D arrayBoard;

	for (int row = 0; row < mHeight; ++row)
	{
		for (int column = 0; column < mWidth; ++column)
		{
			auto tileType = mBoard[row][column]->getTileType();
			switch (tileType)
			{
			case Tile::Type::Obstacle:
				arrayBoard[row * 10 + column] = static_cast<int>(tileType);
				break;

			default:
				arrayBoard[row * 10 + column] = static_cast<int>(Tile::Type::Nothing);
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
	if (mLastCppExecution.has_value() && mLastAsmExecution.has_value())
	{
		switch (mCurrentlyShownAlgorithm)
		{
		case Algorithm::Cpp:
			setBoard(mLastCppExecution.value());
			break;
		case Algorithm::Asm:
			setBoard(mLastAsmExecution.value());
			break;
		}
	}
}

void Board::imGuiSelectAlgorithm()
{
	static const char* labels[] = { "C++", "Assembler" };
	ImGui::TextWrapped("Select algorithm of which result should be displayed");
	if (ImGui::BeginCombo("Algorithm", labels[static_cast<int>(mCurrentlyShownAlgorithm)]))
	{
		for (int n = 0; n < IM_ARRAYSIZE(labels); n++)
		{
			const bool is_selected = (static_cast<int>(mCurrentlyShownAlgorithm) == n);
			if (ImGui::Selectable(labels[n], is_selected))
			{
				mCurrentlyShownAlgorithm = static_cast<Algorithm>(n);
				refreshAlgorithmDisplay();
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

void Board::imGuiDisplayMeasurements()
{
	if (mTimeMeasurementsOfAsm.size() != mTimeMeasurementsOfCpp.size())
		throw std::runtime_error("Asm and Cpp should have the same number of measurements");

	const auto noOfMeasurments = mTimeMeasurementsOfCpp.size();
	std::vector<float> avgOfExecution;

	if(!mTimeMeasurementsOfAsm.empty() && !mTimeMeasurementsOfCpp.empty())
	{
		avgOfExecution.emplace_back(std::accumulate(mTimeMeasurementsOfCpp.cbegin(),
		                        mTimeMeasurementsOfCpp.cend(), 0.0f) / noOfMeasurments);
		avgOfExecution.emplace_back(std::accumulate(mTimeMeasurementsOfAsm.cbegin(),
		                        mTimeMeasurementsOfAsm.cend(), 0.0f) / noOfMeasurments);
	}

	static constexpr int maxValuesToDisplay = 10;

	int numberOfValuesToDisplay = (noOfMeasurments < maxValuesToDisplay) ? noOfMeasurments : maxValuesToDisplay;
	int dataOffset = (noOfMeasurments < maxValuesToDisplay) ? 0 : noOfMeasurments - maxValuesToDisplay;
	ImGui::Begin("Measurments", 0, ImGuiWindowFlags_NoMove);
	if (ImPlot::BeginPlot("Last 10 executions")) {
		ImPlot::SetupAxisFormat(ImAxis_Y1, "%.2f");
		ImPlot::SetupAxes("Executions", "Times (ms)", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
		ImPlot::PlotStems("C++", mTimeMeasurementsOfCpp.data() + dataOffset, numberOfValuesToDisplay);
		ImPlot::PlotStems("ASM", mTimeMeasurementsOfAsm.data() + dataOffset, numberOfValuesToDisplay);

		ImPlot::EndPlot();
	}

	static const char* labels[] = { "C++", "Assembler" };
	static const double positions[] = { 0,1 };
	if (ImPlot::BeginPlot("Average of all previous executions")) {
		ImPlot::SetupAxes("Performance", "Times (ms)", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
		ImPlot::SetupAxisTicks(ImAxis_X1, positions, 2, labels);
		ImPlot::PlotBars("", avgOfExecution.data(), avgOfExecution.size());
		ImPlot::EndPlot();
	}
}

void Board::updateImGui()
{
	imGuiDisplayMeasurements();
	ImGui::Separator();
	imGuiSelectAlgorithm();

	// Select distance function buttons
	// TODO ...

	ImGui::End();
}

Tile& Board::getTile(const sf::Vector2i mousePosition)
{
	int tileIndexX = mousePosition.x / mTileSize;
	int tileIndexY = mousePosition.y / mTileSize;

	return *mBoard[tileIndexX][tileIndexY];
}
