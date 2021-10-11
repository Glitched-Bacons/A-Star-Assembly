#include "Tile.h"

#include <SFML/Graphics/RenderTarget.hpp>

Tile::Tile(Tile::Type typeOfTile, float sizeOfTile)
{
	setTileType(typeOfTile);
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(1.f);
	shape.setSize(sf::Vector2f(sizeOfTile, sizeOfTile));
}

void Tile::setTileType(Tile::Type typeOfTile)
{
	tileType = typeOfTile;

	switch(typeOfTile)
	{
		case Type::Nothing:
		{
			shape.setFillColor(sf::Color::White);
		}
		break;

		case Type::StartingPoint:
		{
			shape.setFillColor(sf::Color::Yellow);
		}
		break;

		case Type::EndingPoint:
		{
			shape.setFillColor(sf::Color::Magenta);
		}
		break;

		case Type::Obstacle:
		{
			shape.setFillColor(sf::Color::Black);
		}
		break;
	}
}

Tile::Type Tile::getTileType() const
{
	return tileType;
}

void Tile::setPosition(sf::Vector2f position)
{
	shape.setPosition(position);
}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(shape, states);
}
