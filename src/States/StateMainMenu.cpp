#include "Asteroids/States/StateMainMenu.hpp"

#include "Asteroids/GUI/Widgets.hpp"
#include "Asteroids/States/StateGameLobby.hpp"
#include "Asteroids/States/StateJoinLobby.hpp"

#include <iostream>

StateMainMenu::StateMainMenu()
{
	//Host Game Button
	Button *button = new Button("HostGameButton", sf::Vector2f(110.0f, 30.0f));
	button->setAnchor(sf::Vector2f(0.02f, 0.02f), sf::Vector2f(), UPPER_LEFT);
	canvas.addChildWidget(button);
	
	Label *label = new Label("HostGameLabel", "Host Game");
	label->setAnchor(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(), MIDDLE);
	button->addChildWidget(label);
	
	button->setFunc([this]()
	{
		currentState.state = new StateGameLobby(true);
		currentState.destroyLast = true;
	});
	
	//Join Game Button
	button = new Button("JoinGameButton", sf::Vector2f(110.0f, 30.0f));
	button->setAnchor(sf::Vector2f(0.02f, 0.02f), sf::Vector2f(0.0f, 40.0f), UPPER_LEFT);
	canvas.addChildWidget(button);
	
	label = new Label("JoinGameLabel", "Join Game");
	label->setAnchor(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(), MIDDLE);
	button->addChildWidget(label);
	
	button->setFunc([this]()
	{
		currentState.state = new StateJoinLobby();
		currentState.destroyLast = true;
	});
	
	//Exit Game Button
	button = new Button("ExitGameButton", sf::Vector2f(110.0f, 30.0f));
	button->setAnchor(sf::Vector2f(0.02f, 0.98f), sf::Vector2f(), LOWER_LEFT);
	canvas.addChildWidget(button);
	
	label = new Label("ExitGameLabel", "Exit Game");
	label->setAnchor(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(), MIDDLE);
	button->addChildWidget(label);
	
	button->setFunc([this]()
	{
		currentState.state = nullptr;
		currentState.destroyLast = true;
	});
}

void StateMainMenu::handleEvent(const sf::Event& event)
{
	State::handleEvent(event);
}

void StateMainMenu::update(float dt)
{
	State::update(dt);
}

void StateMainMenu::draw(sf::RenderWindow& window)
{
	State::draw(window);
}