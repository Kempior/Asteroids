#include <random>
#include <ctime>
#include <utility>
#include "SFML/Window.hpp"

#include "Asteroids/States/StateGame.hpp"
#include "Asteroids/Networking/PacketType.hpp"

StateGame::StateGame(int playerCount, int playerID, sf::TcpSocket* server, std::vector<sf::TcpSocket*> clients, sf::IntRect worldSize) : worldSize(worldSize), playerID(playerID) {

	isHost = !playerID;
	
	this->server = server;
	this->clients = std::move(clients);
	
	atlasTexture.loadFromFile("../../assets/Atlas.png");

	clearColor = sf::Color::Black;

	CreateAsteroids(20);
	for (int i = 0; i < playerCount; ++i) {
		CreateShip({worldSize.left + (worldSize.width / (float)(playerCount + 1)) * (i + 1), worldSize.top + worldSize.height * 0.75f});
	}
}

void StateGame::CreateAsteroids(int howMany) {

	std::mt19937 mt(time(NULL));
	std::uniform_real_distribution<float> xPos(worldSize.left, worldSize.left + worldSize.width);
	std::uniform_real_distribution<float> yPos(worldSize.top, worldSize.top + worldSize.height);

	std::uniform_real_distribution<float> spd(-50, 50);
	std::uniform_real_distribution<float> rot(0, 360.f);

	std::uniform_real_distribution<float> rotSpd(-30.f, 30.f);

	for (int i = 0; i < howMany; ++i) {
		Asteroid newAst ({xPos(mt), yPos(mt)}, {spd(mt), spd(mt)});
		newAst.sprite.setTextureRect({0, 0, 128, 128});
		newAst.sprite.setOrigin(63, 63);

		newAst.rotation = rot(mt);
		newAst.rotationSpeed = rotSpd(mt);

		newAst.sprite.setTexture(atlasTexture);

		asteroids.push_back(newAst);
	}
}

void StateGame::CreateAsteroids(int howMany, sf::Vector2f position) {

	std::mt19937 mt(time(NULL));

	std::uniform_real_distribution<float> spd(-50, 50);
	std::uniform_real_distribution<float> rot(0, 360.f);

	std::uniform_real_distribution<float> rotSpd(-30.f, 30.f);

	for (int i = 0; i < howMany; ++i) {
		Asteroid newAst (position);

		newAst.sprite.setTextureRect({0, 0, 128, 128});
		newAst.sprite.setOrigin(63, 63);

		newAst.rotation = rot(mt);
		newAst.rotationSpeed = rotSpd(mt);

		newAst.sprite.setTexture(atlasTexture);

		asteroids.push_back(newAst);
	}
}

void StateGame::CreateShip(sf::Vector2f position) {

	Ship newShip(position);

	newShip.sprite.setTextureRect({128, 0, 128, 128});
	newShip.sprite.setOrigin(63, 63);

	newShip.sprite.setTexture(atlasTexture);

	ships.push_back(newShip);
}

void StateGame::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
			case sf::Keyboard::W:
			{
				ships[playerID].isAccelarating = true;
				sf::Packet packet;
				packet << PacketType::MOVEFORWARD << playerID;
				sendPacket(packet);
				break;
			}
			default:
				break;
		}
	}
}

void StateGame::update(float dt)
{
	for (auto &roid : asteroids) {
		roid.position += roid.velocity * dt;
		roid.rotation += roid.rotationSpeed * dt;

		//Constraints on x axis
		if (roid.position.x < worldSize.left) {
			roid.velocity.x *= -1;
			roid.position.x += roid.velocity.x * dt;
		}
		else if (worldSize.left + worldSize.width < roid.position.x) {
			roid.velocity.x *= -1;
			roid.position.x += roid.velocity.x * dt;
		}
		//Constraints on y axis
		if (roid.position.y < worldSize.top) {
			roid.velocity.y *= -1;
			roid.position.y += roid.velocity.y * dt;
		}
		else if (worldSize.top + worldSize.height < roid.position.y) {
			roid.velocity.y *= -1;
			roid.position.y += roid.velocity.y * dt;
		}
	}
	for (auto &ship : ships) {
		if (ship.isAccelarating) {
			ships[playerID].velocity += ships[playerID].accelaration * ships[playerID].Forward();
			ship.isAccelarating = false;
		}

		ship.position += ship.velocity * dt;
		ship.rotation += ship.rotationSpeed * dt;

		//Constraints on x axis
		if (ship.position.x < worldSize.left) {
			ship.velocity.x *= -1;
			ship.position.x += ship.velocity.x * dt;
		}
		else if (worldSize.left + worldSize.width < ship.position.x) {
			ship.velocity.x *= -1;
			ship.position.x += ship.velocity.x * dt;
		}
		//Constraints on y axis
		if (ship.position.y < worldSize.top) {
			ship.velocity.y *= -1;
			ship.position.y += ship.velocity.y * dt;
		}
		else if (worldSize.top + worldSize.height < ship.position.y) {
			ship.velocity.y *= -1;
			ship.position.y += ship.velocity.y * dt;
		}
	}
	
	recivePackets();
}

void StateGame::draw(sf::RenderWindow& window)
{
	for (auto &roid : asteroids) {
		roid.Draw(window);
	}
	for (auto &ship : ships) {
		ship.Draw(window);
	}
}

void StateGame::sendPacket(sf::Packet &packet)
{
	if(isHost)
	{
		for(auto client : clients)
		{
			while(client->send(packet) != sf::Socket::Done) {}
		}
	}
	else
	{
		while(server->send(packet) != sf::Socket::Done) {}
	}
}

void StateGame::recivePackets()
{
	if(isHost) //Host
	{
		for(auto client : clients)
		{
			sf::Packet packet;
			while(client->receive(packet) == sf::Socket::Done)
			{
				int packetType;
				packet >> packetType;
				
				switch(packetType)
				{
					case MOVEFORWARD:
						unsigned int id;
						packet >> id;
						ships[id].isAccelarating = true;
						{
							sf::Packet repacket;
							repacket << packetType << id;
							sendPacket(repacket);
						}
						break;
					default:
						break;
				}
			}
		}
	}
	else //Client
	{
		sf::Packet packet;
		while(server->receive(packet) == sf::Socket::Done)
		{
			int packetType;
			packet >> packetType;
			
			switch(packetType)
			{
				case MOVEFORWARD:
					unsigned int id;
					packet >> id;
					ships[id].isAccelarating = true;
					break;
				default:
					break;
			}
		}
	}
}

