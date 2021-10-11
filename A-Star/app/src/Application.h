#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "Board.h"

class Application final
{
public:
	Application();

	void run();
	void processEvents();
	void fixedUpdate(sf::Time deltaTime);
	void updateImGui();
	void render();

private:
	static inline const sf::Time TIME_PER_FRAME = sf::seconds(1.f / 60.f);
	static inline const int SCREEN_EDGE = 640;
	static inline const int SCREEN_HEIGHT = 640;
	static inline const int BOARD_SIZE = 16;

	sf::RenderWindow mWindow;
	Board board;

	bool isGameRunning = true;

};
