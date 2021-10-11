#pragma once

#include <memory>
#include <optional>

#include "Tile.h"

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

	void setBoard(Array1D arrayBoard);
	void setBoard(Array2D arrayBoard);

	Array1D getArray1D() const;
	Array2D getArray2D() const;

	std::optional<sf::Vector2i> getStartingPoint() const;
	std::optional<sf::Vector2i> getEndingPoint() const;

private:
	const float mTileSize;
	const int mWidth;
	const int mHeight;

	Tile& getTile(const sf::Vector2i mousePosition);
	std::vector<std::vector<std::unique_ptr<Tile>>> board;

	std::optional<sf::Vector2i> startingPoint;
	std::optional<sf::Vector2i> endingPoint;
};
