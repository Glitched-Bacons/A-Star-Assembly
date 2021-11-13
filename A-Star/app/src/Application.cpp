#include "Application.h"

#include <imgui-sfml/imgui-SFML.h>
#include <imgui/imgui.h>
#include <implot/implot.h>
#include <SFML/Window/Event.hpp>

Application::Application()
	: mWindow(sf::VideoMode(SCREEN_EDGE, SCREEN_EDGE),
		"A* Algorithms Application", sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close)
	, mBoard(BOARD_SIZE, BOARD_SIZE, SCREEN_EDGE / BOARD_SIZE)
{

	// Limit the framerate to 60 frames per second
	mWindow.setFramerateLimit(60);
	mWindow.setActive(true);

	ImGui::SFML::Init(mWindow);
	ImPlot::CreateContext();
}

void Application::run()
{
	sf::Clock clock;
	auto frameTimeElapsed = sf::Time::Zero;
	while (mIsAppRunning)
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
	ImPlot::DestroyContext();
	ImGui::SFML::Shutdown();
}

void Application::updateImGui()
{
	ImGui::SetNextWindowPos(ImVec2(mWindow.getSize().x - mWindow.getSize().x / 3, 0), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(mWindow.getSize().x / 3, mWindow.getSize().y), ImGuiCond_Once);
	mBoard.updateImGui();
}

void Application::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			mIsAppRunning = false;

		ImGui::SFML::ProcessEvent(event);

		// Ignore all events that are related directly with ImGui
		if (ImGui::IsWindowHovered(ImGuiFocusedFlags_AnyWindow) || ImGui::IsAnyItemActive())
			return;

		// TODO: App should handle processes here
		mBoard.processEvent(event);
	}
}

void Application::fixedUpdate(sf::Time deltaTime)
{
	auto deltaTimeInSeconds = deltaTime.asSeconds();


	// TODO: App should handle updates here
	mBoard.updateMouse(sf::Mouse::getPosition(mWindow));
	mBoard.fixedUpdate(deltaTimeInSeconds);
}

void Application::render()
{
	mWindow.clear();

	// TODO: App should handle renders here
	mBoard.draw(mWindow, sf::Transform::Identity);

	mWindow.pushGLStates();
	ImGui::SFML::Render(mWindow);
	mWindow.popGLStates();

	mWindow.display();
}
