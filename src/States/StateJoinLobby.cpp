#include "Asteroids/States/StateJoinLobby.hpp"

#include "Asteroids/GUI/Widgets.hpp"
#include "Asteroids/States/StateGameLobby.hpp"
#include "Asteroids/States/StateMainMenu.hpp"

#include "SFML/Network.hpp"

#include <regex>
#include <iostream>

StateJoinLobby::StateJoinLobby()
{
	//Connect Button
	Button *button = new Button("ConnectButton", sf::Vector2f(110.0f, 30.0f));
	button->setAnchor(sf::Vector2f(0.02f, 0.02f), sf::Vector2f(), UPPER_LEFT);
	canvas.addChildWidget(button);
	
	Label *label = new Label("ConnectLabel", "Connect");
	label->setAnchor(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(), MIDDLE);
	button->addChildWidget(label);
	
	button->setFunc([this]()
	{
		sf::TcpSocket *socket = new sf::TcpSocket();
		if(socket->connect(ip, 34197, sf::seconds(5)) == sf::Socket::Done)
		{
			currentState.state = new StateGameLobby(socket);
			currentState.destroyLast = true;
		}
		else
		{
			canvas.getWidget<Label>("ErrorLabel")->setText("Connection error");
			delete socket;
		}
	});
	
	//Back Button
	button = new Button("BackButton", sf::Vector2f(110.0f, 30.0f));
	button->setAnchor(sf::Vector2f(0.02f, 0.02f), sf::Vector2f(0.0f, 40.0f), UPPER_LEFT);
	canvas.addChildWidget(button);
	
	label = new Label("BackLabel", "Back");
	label->setAnchor(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(), MIDDLE);
	button->addChildWidget(label);
	
	button->setFunc([this]()
	{
		currentState.state = new StateMainMenu();
		currentState.destroyLast = true;
	});
	
	//IP Text Field
	TextField *textField = new TextField("IPTextField", sf::Vector2f(120.0f, 30.0f));
	textField->setAnchor(sf::Vector2f(0.02f, 0.02f), sf::Vector2f(120.0f, 00.0f), UPPER_LEFT);
	canvas.addChildWidget(textField);
	textField->setTextSourceFunc([this]() -> std::string
	{
		return ip;
	});
	textField->setDataUpdateFunc([this, textField]()
	{
		ip = textField->getText();
	});
	textField->setValidateFunc([](std::string str) -> bool
	{
		return std::regex_match(str, std::regex("(?:(?:25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9][0-9]|[0-9])\\.){3}(?:25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9][0-9]|[0-9])"));
	});
	
	label = new Label("IPLabel", "IP Address");
	label->setAnchor(sf::Vector2f(1.0f, 0.5f), sf::Vector2f(5.0f, 0.0f), LEFT);
	textField->addChildWidget(label);
	
	//Connection error
	label = new Label("ErrorLabel", "");
	label->setAnchor(sf::Vector2f(0.02f, 0.02f), sf::Vector2f(120.0f, 40.0f), UPPER_LEFT);
	label->setColor(sf::Color::Red);
	canvas.addChildWidget(label);
}

void StateJoinLobby::handleEvent(const sf::Event& event)
{
	State::handleEvent(event);
}

void StateJoinLobby::update(float dt)
{
	State::update(dt);
}

void StateJoinLobby::draw(sf::RenderWindow& window)
{
	State::draw(window);
}