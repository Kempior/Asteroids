#include <random>
#include <ctime>
#include <utility>
#include "SFML/Window.hpp"

#include "Asteroids/States/StateGame.hpp"
#include "Asteroids/Networking/PacketType.hpp"
#include "Asteroids/States/StateMainMenu.hpp"

#include <thread>
#include <chrono>

StateGame::StateGame(int playerCount, int playerID, sf::TcpSocket* server, std::vector<sf::TcpSocket*> clients, sf::IntRect worldSize) : worldSize(worldSize), playerID(playerID), mt(time(0)) {

	isHost = !playerID;
	
	this->server = server;
	this->clients = std::move(clients);
	
	atlasTexture.loadFromFile("../../assets/Atlas.png");

	clearColor = sf::Color::Black;

	for (int i = 0; i < playerCount; ++i) {
		CreateShip({worldSize.left + (worldSize.width / (float)(playerCount + 1)) * (i + 1), worldSize.top + worldSize.height * 0.75f});
	}

	ships[playerID].sprite.setTextureRect({128, 128, 128, 128});

	if(isHost)
	{
		CreateAsteroids(10);
	}
}

StateGame::~StateGame()
{
	delete server;
	for(auto client : clients)
	{
		delete client;
	}
}

Asteroid StateGame::CreateAsteroid(sf::Vector2f position, sf::Vector2f velocity, float rotation, float rotationSpeed, float scale) {

	Asteroid newAst(position, velocity, rotation, rotationSpeed);

	newAst.sprite.setTextureRect({0, 0, 128, 128});
	newAst.sprite.setOrigin(63, 63);

	newAst.sprite.setTexture(atlasTexture);

	newAst.sprite.scale({scale * 1.f, scale * 1.f});
	newAst.radius = scale * 64;

	return newAst;
}

void StateGame::CreateAsteroids(int howMany) {
	
	std::uniform_real_distribution<float> xPos(worldSize.left, worldSize.left + worldSize.width);
	std::uniform_real_distribution<float> yPos(worldSize.top, worldSize.top + worldSize.height);

	std::uniform_real_distribution<float> spd(-50, 50);
	std::uniform_real_distribution<float> rot(0, 360.f);

	std::uniform_real_distribution<float> rotSpd(-30.f, 30.f);

	for (int i = 0; i < howMany; ++i) {
		Asteroid newAst = CreateAsteroid({xPos(mt), yPos(mt)}, {spd(mt), spd(mt)}, rot(mt), rotSpd(mt), 1);

		// Rerolls if asteroid is spawning on or near a player
		bool reroll = false;
		for (auto &ship : ships) {
			if (newAst.Collide(ship, 200.f)) {
				reroll = true;
				break;
			}
		}
		if (reroll) {
			i--;
			continue;
		}

		sf::Packet packet;
		packet << PacketType::CREATEASTEROID << newAst.position.x << newAst.position.y << newAst.velocity.x << newAst.velocity.y << newAst.rotation << newAst.rotationSpeed;
		sendPacket(packet);
		
		asteroids.push_back(newAst);
	}
}

void StateGame::CreateAsteroids(int howMany, sf::Vector2f position, float scale) {

	std::uniform_real_distribution<float> spd(-50, 50);
	std::uniform_real_distribution<float> rot(0, 360.f);

	std::uniform_real_distribution<float> rotSpd(-30.f, 30.f);

	for (int i = 0; i < howMany; ++i) {
		asteroids.push_back(CreateAsteroid(position, {spd(mt), spd(mt)}, rot(mt), rotSpd(mt), 1));
	}
}

void StateGame::CreateShip(sf::Vector2f position, float scale) {

	Ship newShip(position);

	newShip.sprite.setTextureRect({128, 0, 128, 128});
	newShip.sprite.setOrigin(63, 63);

	newShip.sprite.setTexture(atlasTexture);

	newShip.sprite.scale({0.5f * scale, 0.5f * scale});
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
	if(event.type == sf::Event::Closed)
	{
		sf::Packet packet;
		packet << PacketType::DISCONNECT;
		sendPacket(packet);
		
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		
		if(isHost)
		{
			for(auto client : clients)
			{
				client->disconnect();
			}
		}
		else
		{
			server->disconnect();
		}
	}
	
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
		if (roid.isDestroyed)
			continue;

		roid.position += roid.velocity * dt;
		roid.rotation += roid.rotationSpeed * dt;

		roid.Constrain(worldSize, dt);
	}
	
	for (auto &ship : ships) {
		if (ship.isDestroyed)
			continue;

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

			ship.remainingDelay = ship.shotDelay * (0.3f + 0.7f * (1.f / wave));
		}

		ship.remainingDelay -= dt;
	}

	// Erases projectiles after their lifetime expires
	for (int i = 0; i < projectiles.size(); i++) {

		projectiles[i].lifetime -= dt;

		if (projectiles[i].lifetime <= 0)
			projectiles.erase(projectiles.begin() + i);

		if (projectiles[i].isDestroyed)
			continue;

		projectiles[i].position += projectiles[i].velocity * dt;
		projectiles[i].rotation += projectiles[i].rotationSpeed * dt;
	}

	// Collisions between projectiles and asteroids
	for (int i = 0; i < projectiles.size(); ++i) {
		for (int j = 0; j < asteroids.size(); ++j) {
			if (projectiles[i].Collide(asteroids[j])) {
				//CreateAsteroids(2, asteroids[j].position, asteroids[j].sprite.getScale().x / 2);

				projectiles.erase(projectiles.begin() + i);
				asteroids.erase(asteroids.begin() + j);
			}
		}
	}

	if (asteroids.empty()) {
		if(isHost)
		{
			CreateAsteroids(10 * wave);
			wave++;

			for(auto &ship : ships)
			{
				if(ship.isDestroyed)
				{
					ship.isDestroyed = false;
					ship.velocity = {};
				}
			}
		}
		
		sf::Packet packet;
		packet << PacketType::NEWWAVE;
		sendPacket(packet);
	}

	// Collisions between player and asteroids
	for (auto &asteroid : asteroids) {
		if (!ships[playerID].isDestroyed && ships[playerID].Collide(asteroid)) {
			
			
			sf::Packet packet;ships[playerID].isDestroyed = true;
			packet << PacketType::SHIPDESTROYED << playerID;
			sendPacket(packet);
		}
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
			while(client->send(packet) == sf::Socket::Partial) {}
		}
	}
	else
	{
		while(server->send(packet) == sf::Socket::Partial) {}
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
					case DISCONNECT:
					{
						client->disconnect();
						
						break;
					}
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
						break;
					}
					case SHIPDESTROYED:
					{
						unsigned int id;
						packet >> id;
						
						ships[id].isDestroyed = true;
						
						sf::Packet repacket;
						repacket << PacketType::SHIPDESTROYED << id;
						sendPacket(repacket, client);
						
						break;
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
				case DISCONNECT:
				{
					server->disconnect();
					
					currentState.state = new StateMainMenu();
					currentState.destroyLast = true;
					
					break;
				}
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
					break;
				}
				case SHIPDESTROYED:
				{
					unsigned int id;
					packet >> id;
					
					ships[id].isDestroyed = true;
					
					break;
				}
				case CREATEASTEROID:
				{
					sf::Vector2f position;
					sf::Vector2f velocity;
					float rot, rotSpeed;
					packet >> position.x >> position.y >> velocity.x >> velocity.y >> rot >> rotSpeed;
					asteroids.push_back(CreateAsteroid(position, velocity, rot, rotSpeed, 1));
					break;
				}
				case NEWWAVE:
				{
					wave++;

					for(auto &ship : ships)
					{
						if(ship.isDestroyed)
						{
							ship.isDestroyed = false;
							ship.velocity = {};
						}
					}
				}
				default:
					break;
			}
		}
	}
}


