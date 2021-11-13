#pragma once

#include <memory>
#include <optional>

#include "Tile.h"

namespace sf
{
	class Event;
}

enum class Algorithm
{
	Cpp,
	Asm
};

class Board
{
public:
	using Array1D = std::vector<int>;
	using Array2D = std::vector<std::vector<int>>;

	Board(int width, int height, float tileSize);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void processEvent(const sf::Event& event);
	void fixedUpdate(float deltaTime);
	void updateMouse(sf::Vector2i mousePosition);
	void updateImGui();

private:
	void setBoard(Array1D arrayBoard);
	void setBoard(Array2D arrayBoard);

	[[nodiscard]] Array1D getArray1D() const;
	[[nodiscard]] Array2D getArray2D() const;

	[[nodiscard]] std::optional<sf::Vector2i> getStartingPoint() const;
	[[nodiscard]] std::optional<sf::Vector2i> getEndingPoint() const;
	Tile& getTile(const sf::Vector2i mousePosition);

	void refreshAlgorithmDisplay();
	void imGuiSelectAlgorithm();
	void imGuiDisplayMeasurements();
	void refreshAStarAlgorithm();

private:
	const float mTileSize;
	const int mWidth;
	const int mHeight;
	bool mShouldObstaclesBeRemoved = false;
	Algorithm mCurrentlyShownAlgorithm;

	std::vector<std::vector<std::unique_ptr<Tile>>> mBoard;

	std::optional<sf::Vector2i> mStartingPoint;
	std::optional<sf::Vector2i> mEndingPoint;

	std::optional<Array2D> mLastAsmExecution;
	std::optional<Array2D> mLastCppExecution;

	std::vector<float> mTimeMeasurementsOfCpp;
	std::vector<float> mTimeMeasurementsOfAsm;
};
