#include "Tile.h"

#include <SFML/Graphics/RenderTarget.hpp>

Tile::Tile(Tile::Type typeOfTile, float sizeOfTile)
{
	setTileType(typeOfTile);
	mShape.setOutlineColor(sf::Color::Black);
	mShape.setOutlineThickness(1.f);
	mShape.setSize(sf::Vector2f(sizeOfTile, sizeOfTile));
}

void Tile::setTileType(Tile::Type typeOfTile)
{
	mTileType = typeOfTile;

	switch(typeOfTile)
	{
		case Type::Nothing:
		{
			mShape.setFillColor(sf::Color::White);
		}
		break;

		case Type::Unvisited:
		{
			mShape.setFillColor(sf::Color(50, 50, 50));
		}
		break;

		case Type::Visited:
		{
			mShape.setFillColor(sf::Color(50, 50, 100));
		}
		break;

		case Type::Path:
		{
			mShape.setFillColor(sf::Color::Green);
		}
		break;

		case Type::StartingPoint:
		{
			mShape.setFillColor(sf::Color::Yellow);
		}
		break;

		case Type::EndingPoint:
		{
			mShape.setFillColor(sf::Color::Magenta);
		}
		break;

		case Type::Obstacle:
		{
			mShape.setFillColor(sf::Color::Black);
		}
		break;
	}
}

Tile::Type Tile::getTileType() const
{
	return mTileType;
}

void Tile::setPosition(sf::Vector2f position)
{
	mShape.setPosition(position);
}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mShape, states);
}
