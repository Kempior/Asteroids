#ifndef ASTEROIDS_ASTEROID_HPP
#define ASTEROIDS_ASTEROID_HPP

#include "GameObject.hpp"

class Asteroid : public GameObject {
public:
	explicit Asteroid(sf::Vector2f position = sf::Vector2f(), sf::Vector2f velocity = sf::Vector2f(), float rotation = 0.f, float rotationSpeed = 0.f)
			: GameObject(position, velocity, rotation, rotationSpeed) {}

	void Draw(sf::RenderWindow &window) override;
};


#endif //ASTEROIDS_ASTEROID_HPP
