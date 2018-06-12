#ifndef ASTEROIDS_STATEGAME_HPP
#define ASTEROIDS_STATEGAME_HPP

#include "Asteroids/States/State.hpp"
#include "Asteroids/GameObjects/Ship.hpp"
#include "Asteroids/GameObjects/Asteroid.hpp"
#include "Asteroids/GameObjects/Projectile.hpp"

#include "SFML/Network.hpp"

#include <vector>

class StateGame : public State
{
	const int playerID;
	bool isHost;
	sf::TcpSocket *server;
	std::vector<sf::TcpSocket*> clients;

	const sf::IntRect worldSize;

	std::vector<Ship> ships;
	std::vector<Asteroid> asteroids;
	std::vector<Projectile> projectiles;

	sf::Texture atlasTexture;

public:
	//Yes, I'm lazy;
	explicit StateGame(int playerCount, int playerID, sf::TcpSocket* server, std::vector<sf::TcpSocket*> clients, sf::IntRect worldSize = sf::IntRect(0, 0, 1600, 900));
	~StateGame() override;

	void CreateAsteroids(int howMany, unsigned seed = (unsigned)time(nullptr));
	void CreateAsteroids(int howMany, sf::Vector2f position, float scale = 1.f, unsigned seed = (unsigned)time(nullptr));

	void CreateShip(sf::Vector2f position, float scale = 1.f);

	void CreateProjectile(sf::Vector2f position, sf::Vector2f velocity, float lifetime);

	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;

private:
	void sendPacket(sf::Packet& packet, sf::TcpSocket* exclude = nullptr);
	void recivePackets();
};

#endif //ASTEROIDS_STATEGAME_HPP