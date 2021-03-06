#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

class Tile
{
public:
	enum class Type : uint8_t
	{
		Nothing			= 0,
		Obstacle		= 1,
		Visited			= 2,
		Unvisited		= 3,
		Path			= 4,

		// Actually not used in the export
		StartingPoint	= 5,
		EndingPoint		= 6
	};

	Tile(Tile::Type typeOfTile, float sizeOfTile);
	void setTileType(Tile::Type typeOfTile);
	Tile::Type getTileType() const;
	void setPosition(sf::Vector2f position);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

	Tile::Type mTileType;
	sf::RectangleShape mShape;
};