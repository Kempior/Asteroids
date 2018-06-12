#ifndef ASTEROIDS_STATEGAME_HPP
#define ASTEROIDS_STATEGAME_HPP

#include "Asteroids/States/State.hpp"
#include "Asteroids/GameObjects/Asteroid.hpp"
#include "Asteroids/GameObjects/Ship.hpp"

class StateGame : public State
{
	const int playerID;

	const sf::IntRect worldSize;

	std::vector<Ship> ships;
	std::vector<Asteroid> asteroids;

	sf::Texture atlasTexture;

public:
	//Yes, I'm lazy;
	explicit StateGame(int playerCount, int playerID, sf::IntRect worldSize = sf::IntRect(0, 0, 1600, 900));

	void CreateAsteroids(int howMany);
	void CreateAsteroids(int howMany, sf::Vector2f position);

	void CreateShip(sf::Vector2f position);

	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;
};

#endif //ASTEROIDS_STATEGAME_HPP