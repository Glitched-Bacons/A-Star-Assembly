#include "Application.h"

#include <imgui-sfml/imgui-SFML.h>
#include <SFML/Window/Event.hpp>

Application::Application()
	: mWindow(sf::VideoMode(SCREEN_EDGE, SCREEN_EDGE),
		"A* Algorithms Application", sf::Style::Titlebar | sf::Style::Close)
	, board(BOARD_SIZE, BOARD_SIZE, SCREEN_EDGE / BOARD_SIZE)
{

	// Limit the framerate to 60 frames per second
	mWindow.setFramerateLimit(60);
	mWindow.setActive(true);

	ImGui::SFML::Init(mWindow);
}

void Application::run()
{
	sf::Clock clock;
	auto frameTimeElapsed = sf::Time::Zero;
	while (isGameRunning)
	{
		frameTimeElapsed += clock.restart();

		while (frameTimeElapsed > TIME_PER_FRAME)
		{
			// Update world no more than 60 frames per seconds
			frameTimeElapsed -= TIME_PER_FRAME;
			processEvents();
			fixedUpdate(TIME_PER_FRAME);
		}

		ImGui::SFML::Update(mWindow, frameTimeElapsed);
		updateImGui();
		render();
	}

	mWindow.close();
	ImGui::SFML::Shutdown();
}

void Application::updateImGui()
{
	
}

void Application::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			isGameRunning = false;

		ImGui::SFML::ProcessEvent(event);

		// TODO: App should handle processes here
		board.processEvent(event);
	}
}

void Application::fixedUpdate(sf::Time deltaTime)
{
	auto deltaTimeInSeconds = deltaTime.asSeconds();


	// TODO: App should handle updates here
	board.fixedUpdate(deltaTimeInSeconds);
}

void Application::render()
{
	mWindow.clear();

	// TODO: App should handle renders here
	board.draw(mWindow, sf::Transform::Identity);

	mWindow.pushGLStates();
	ImGui::SFML::Render(mWindow);
	mWindow.popGLStates();

	mWindow.display();
}
