#ifndef ASTEROIDS_SHIP_HPP
#define ASTEROIDS_SHIP_HPP

#include "GameObject.hpp"

class Ship : public GameObject {
public:
	explicit Ship(sf::Vector2f position = sf::Vector2f(), sf::Vector2f velocity = sf::Vector2f(), float rotation = 0.f, float rotationSpeed = 0.f)
			: GameObject(position, velocity, rotation, rotationSpeed) {}

	void Draw(sf::RenderWindow &window) override;

	float accelaration;
};


#endif //ASTEROIDS_SHIP_HPP
