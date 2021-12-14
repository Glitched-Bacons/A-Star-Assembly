#pragma once

#include <map>
#include <memory>
#include <optional>
#include <AStarCpp/AStar.h>

#include "Tile.h"
#include "ImguiDisplay/StatisticDisplayer.h"
#include "Measurements/PerformanceMeasurer.h"

namespace as
{
	namespace AstarModernCpp
	{
		enum class Distance;
	}
}

namespace sf
{
	class Event;
}

class Board
{
public:
	using Array1D = std::vector<int>;
	using Array2D = std::vector<std::vector<int>>;

	Board(int width, int height, float tileSize);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void processEvent(const sf::Event& event);
	void fixedUpdate(float deltaTime);
	void updateMouse(const sf::Vector2i& mousePosition);
	void updateImGui();

private:
	void setBoard(Array1D arrayBoard);
	void setBoard(Array2D arrayBoard);

	[[nodiscard]] Array1D getArray1D() const;
	[[nodiscard]] Array2D getArray2D() const;

	[[nodiscard]] std::optional<sf::Vector2i> getStartingPoint() const;
	[[nodiscard]] std::optional<sf::Vector2i> getEndingPoint() const;

	Tile& getTile(const sf::Vector2i& mousePosition);
	[[nodiscard]] std::vector<AlgorithmMeasurments> getMeasurements();

	void refreshAlgorithmDisplay();
	void refreshAStarAlgorithm();
	bool isPositionInBoard(const sf::Vector2i& clickedPosition);

private:
	const float mTileSize;
	const int mWidth;
	const int mHeight;
	bool mShouldObstaclesBeRemoved;
	static constexpr int maxMeasurmentsDisplay = 10;

	Algorithm mCurrentlyShownAlgorithm;
	as::AstarModernCpp::Distance mDistanceAlgorithm;

	std::vector<std::vector<std::unique_ptr<Tile>>> mBoard;
	StatisticDisplayer statisticDisplayer;

	std::optional<sf::Vector2i> mStartingPoint;
	std::optional<sf::Vector2i> mEndingPoint;

	std::multimap<Algorithm, sf::Time> mAlgorithmsExecutions;
	std::map<Algorithm, AnyArray> mLastAlgorithmExecutions;
};
