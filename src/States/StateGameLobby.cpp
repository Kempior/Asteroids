#include "Asteroids/States/StateGameLobby.hpp"

#include "Asteroids/GUI/Widgets.hpp"
#include "Asteroids/States/StateGame.hpp"
#include "Asteroids/Networking/PacketType.hpp"

#include "SFML/Network.hpp"

#include <iostream>

StateGameLobby::StateGameLobby(bool isHost):
isHost(isHost)
{
	if(isHost)
	{
		playerCount = 1;
		
		listener = new sf::TcpListener();
		listener->setBlocking(false);
		listener->listen(34197);
		
		clients.push_back(new sf::TcpSocket());
		clients.back()->setBlocking(false);
		
		//Start Button
		Button *button = new Button("StartButton", sf::Vector2f(110.0f, 30.0f));
		button->setAnchor(sf::Vector2f(0.98f, 0.98f), sf::Vector2f(), LOWER_RIGHT);
		canvas.addChildWidget(button);
		
		Label *label = new Label("StartLabel", "Start");
		label->setAnchor(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(), MIDDLE);
		button->addChildWidget(label);
		
		button->setFunc([this]()
		{
			delete clients.back();
			clients.pop_back();
			
			unsigned int id = 1;
			
			for(auto client : clients)
			{
				sf::Packet packet;
				packet << PacketType::START << id++;
				
				while(client->send(packet) != sf::Socket::Done) {}
			}
			
			currentState.state = new StateGame(playerCount, 0);
			currentState.destroyLast = true;
		});
	}
	
	Label *label = new Label("PlayerCount", "");
	label->setAnchor(sf::Vector2f(0.02f, 0.02f), sf::Vector2f(), UPPER_LEFT);
	canvas.addChildWidget(label);
	
	label->setTextSourceLambda([this]()
	{
		return "Player count: " + std::to_string(playerCount);
	});
}

StateGameLobby::StateGameLobby(sf::TcpSocket *socket): StateGameLobby(false)
{
	server = socket;
	server->setBlocking(false);
}

StateGameLobby::~StateGameLobby()
{
	delete listener;
	delete server;
	
	for(auto client : clients)
	{
		delete client;
	}
}

void StateGameLobby::handleEvent(const sf::Event& event)
{
	State::handleEvent(event);
}

void StateGameLobby::update(float dt)
{
	if(isHost) //Host
	{
		if(listener->accept(*clients.back()) == sf::Socket::Done)
		{
			for(auto client : clients)
			{
				sf::Packet packet;
				packet << PacketType::PLAYERCOUNT << static_cast<unsigned int>(clients.size() + 1);
				while(client->send(packet) != sf::Socket::Done) {}
			}
			
			clients.push_back(new sf::TcpSocket());
			clients.back()->setBlocking(false);
			
			playerCount = static_cast<unsigned int>(clients.size());
		}
	}
	else //Client
	{
		sf::Packet packet;
		if(server->receive(packet) == sf::Socket::Done)
		{
			int packetType;
			packet >> packetType;
			switch(packetType)
			{
				case PLAYERCOUNT:
				{
					packet >> playerCount;
					break;
				}
				case START:
				{
					unsigned int id;
					packet >> id;
					currentState.state = new StateGame(playerCount, id);
					currentState.destroyLast = true;
					break;
				}
				default:
				{
					break;
				}
			}
		}
	}
	
	State::update(dt);
}

void StateGameLobby::draw(sf::RenderWindow& window)
{
	State::draw(window);
}