#include "Board.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>


Board::Board(int width, int height, float tileSize)
	: mTileSize(tileSize)
	, mWidth(width)
	, mHeight(height)
{
	board.resize(mHeight);
	for (auto& row : board)
	{
		row.resize(mWidth);
	}

	for (int row = 0; row < mHeight; ++row)
	{
		for (int column = 0; column < mWidth; ++column)
		{
			auto tile = std::make_unique<Tile>(Tile::Type::Nothing, tileSize);;
			tile->setPosition(sf::Vector2f(row * tileSize, column * tileSize));

			board[row][column] = std::move(tile);
		}
	}
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& row : board)
	{
		for (auto& column : row)
		{
			column->draw(target, states);
		}
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
			if (startingPoint.has_value())
			{
				getTile(startingPoint.value()).setTileType(Tile::Type::Nothing);
			}
			selectedTile.setTileType(Tile::Type::StartingPoint);
			startingPoint = clickedPosition;
		}
		else if (event.mouseButton.button == sf::Mouse::Right)
		{
			if (endingPoint.has_value())
			{
				getTile(endingPoint.value()).setTileType(Tile::Type::Nothing);
			}
			selectedTile.setTileType(Tile::Type::EndingPoint);
			endingPoint = clickedPosition;
		}
		else if (event.mouseButton.button == sf::Mouse::Middle)
		{
			if (selectedTile.getTileType() == Tile::Type::Obstacle)
				selectedTile.setTileType(Tile::Type::Nothing);
			else
				selectedTile.setTileType(Tile::Type::Obstacle);
		}
	}
}

void Board::fixedUpdate(float deltaTime)
{

}

void Board::setBoard(Array1D arrayBoard)
{
	for (int row = 0; row < mHeight; ++row)
	{
		for (int column = 0; column < mWidth; ++column)
		{
			auto& tileNumber = arrayBoard[row * 10 + column];
			board[row][column]->setTileType(static_cast<Tile::Type>(tileNumber));
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
			board[row][column]->setTileType(static_cast<Tile::Type>(tileNumber));
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
			auto tileType = board[row][column]->getTileType();
			arrayBoard[row * 10 + column] = static_cast<int>(tileType);
		}
	}

	return arrayBoard;
}

typename Board::Array2D Board::getArray2D() const
{
	Array2D arrayBoard;

	for (int row = 0; row < mHeight; ++row)
	{
		for (int column = 0; column < mWidth; ++column)
		{
			auto tileType = board[row][column]->getTileType();
			arrayBoard[row][column] = static_cast<int>(tileType);
		}
	}

	return arrayBoard;
}

std::optional<sf::Vector2i> Board::getStartingPoint() const
{
	return startingPoint;
}

std::optional<sf::Vector2i> Board::getEndingPoint() const
{
	return endingPoint;
}

Tile& Board::getTile(const sf::Vector2i mousePosition)
{
	int tileIndexX = mousePosition.x / mTileSize;
	int tileIndexY = mousePosition.y / mTileSize;

	return *board[tileIndexX][tileIndexY];
}
