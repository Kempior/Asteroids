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

	ships[playerID].sprite.setTextureRect({128, 128, 128, 128});
}

void StateGame::CreateAsteroids(int howMany) {

	std::mt19937 mt((unsigned)time(nullptr));
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

		newAst.sprite.scale({1.f, 1.f});
		newAst.radius = 64;

		asteroids.push_back(newAst);
	}
}

void StateGame::CreateAsteroids(int howMany, sf::Vector2f position) {

	std::mt19937 mt((unsigned)time(nullptr));

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

	newShip.sprite.scale({0.5, 0.5});
	newShip.radius = 32;

	ships.push_back(newShip);
}

void StateGame::CreateProjectile(sf::Vector2f position, sf::Vector2f velocity, float lifetime) {

	// Please ignore the... thing... below
	Projectile boolet(position, velocity, 90 + atan2f(velocity.y, velocity.x) * (180 / (float)M_PI));
	boolet.lifetime = lifetime;

	boolet.sprite.setTexture(atlasTexture);
	boolet.sprite.setTextureRect({0, 128, 64, 64});

	boolet.sprite.setOrigin(32, 32);

	projectiles.push_back(boolet);
}

void StateGame::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
			case sf::Keyboard::W:
				ships[playerID].isAccelerating = true;
				break;
			case sf::Keyboard::A:
				ships[playerID].isRotatingLeft = true;
				break;
			case sf::Keyboard::D:
				ships[playerID].isRotatingRight = true;
				break;
			case sf::Keyboard::Space:
				ships[playerID].isShooting = true;
				break;
			default:
				break;
		}
	}
	else if (event.type == sf::Event::KeyReleased) {
		switch (event.key.code) {
			case sf::Keyboard::W:
				ships[playerID].isAccelerating = false;
				break;
			case sf::Keyboard::A:
				ships[playerID].isRotatingLeft = false;
				break;
			case sf::Keyboard::D:
				ships[playerID].isRotatingRight = false;
				break;
			case sf::Keyboard::Space:
				ships[playerID].isShooting = false;
				break;
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

		roid.Constrain(worldSize, dt);
	}
	
	for (auto &ship : ships) {
		if (ship.isAccelerating) {
			ship.velocity += ship.acceleration * ship.Forward();
		}

		if (ship.isRotatingLeft && !ship.isRotatingRight) {
			ship.rotationSpeed = -ship.rotationSteering;
		}
		else if (!ship.isRotatingLeft && ship.isRotatingRight) {
			ship.rotationSpeed = ship.rotationSteering;
		}
		else {
			ship.rotationSpeed = 0.f;
		}

		ship.position += ship.velocity * dt;
		ship.rotation += ship.rotationSpeed * dt;

		ship.Constrain(worldSize, dt);

		if (ship.isShooting && ship.remainingDelay <= 0.f) {

			sf::Vector2f position = ship.position + ship.Forward() * ship.radius;
			sf::Vector2f velocity = ship.velocity + ship.Forward() * ship.bulletSpeed;
			
			CreateProjectile(position, velocity, ship.bulletLifetime);
			
			sf::Packet packet;
			packet << PacketType::PROJECTILESPAWN << position.x << position.y << velocity.x << velocity.y << ship.bulletLifetime;
			sendPacket(packet);

			ship.remainingDelay = ship.shotDelay;
		}

		ship.remainingDelay -= dt;
	}

	for (auto &boolet : projectiles) {
		boolet.position += boolet.velocity * dt;
		boolet.rotation += boolet.rotationSpeed * dt;
	}
	
	auto *ship = &ships[playerID];
	sf::Packet packet;
	packet << PacketType::TRANSFORM << playerID << ship->position.x << ship->position.y << ship->velocity.x << ship->velocity.y << ship->rotation << ship->rotationSpeed;
	sendPacket(packet);
	
	recivePackets();
}

void StateGame::draw(sf::RenderWindow& window)
{
	for (auto &ship : ships) {
		ship.Draw(window);
	}
	for (auto &roid : asteroids) {
		roid.Draw(window);
	}
	for (auto &boolet : projectiles) {
		boolet.Draw(window);
	}
}

void StateGame::sendPacket(sf::Packet &packet, sf::TcpSocket* exclude)
{
	if(isHost)
	{
		for(auto client : clients)
		{
			if(client == exclude) continue;
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
					case TRANSFORM:
					{
						unsigned int id;
						packet >> id;
						
						auto *ship = &ships[id];
						packet >> ship->position.x >> ship->position.y >> ship->velocity.x >> ship->velocity.y >> ship->rotation >> ship->rotationSpeed;
						
						sf::Packet repacket;
						repacket << packetType << id << ship->position.x << ship->position.y << ship->velocity.x << ship->velocity.y << ship->rotation << ship->rotationSpeed;
						sendPacket(repacket, client);
						
						break;
					}
					case PROJECTILESPAWN:
					{
						sf::Vector2f position;
						sf::Vector2f velocity;
						float lifetime;
						
						packet >> position.x >> position.y >> velocity.x >> velocity.y >> lifetime;
						CreateProjectile(position, velocity, lifetime);
						
						sf::Packet repacket;
						repacket << PacketType::PROJECTILESPAWN << position.x << position.y << velocity.x << velocity.y << lifetime;
						sendPacket(repacket, client);
					}
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
				case TRANSFORM:
				{
					unsigned int id;
					packet >> id;
					auto *ship = &ships[id];
					packet >> ship->position.x >> ship->position.y >> ship->velocity.x >> ship->velocity.y >> ship->rotation >> ship->rotationSpeed;
					break;
				}
				case PROJECTILESPAWN:
				{
					sf::Vector2f position;
					sf::Vector2f velocity;
					float lifetime;
					
					packet >> position.x >> position.y >> velocity.x >> velocity.y >> lifetime;
					CreateProjectile(position, velocity, lifetime);
				}
				default:
					break;
			}
		}
	}
}

