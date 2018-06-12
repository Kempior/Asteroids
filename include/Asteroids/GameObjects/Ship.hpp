#ifndef ASTEROIDS_SHIP_HPP
#define ASTEROIDS_SHIP_HPP

#include "GameObject.hpp"

class Ship : public GameObject {
public:
	explicit Ship(sf::Vector2f position = sf::Vector2f(), sf::Vector2f velocity = sf::Vector2f(), float rotation = 180.f, float rotationSpeed = 0.f)
			: GameObject(position, velocity, rotation, rotationSpeed) {}

	bool isAccelerating = false;
	bool isRotatingRight = false;
	bool isRotatingLeft = false;

	bool isShooting = false;
	float shotDelay = 0.5f;
	float remainingDelay = 0.0f;
	float bulletSpeed = 400.f;
	float bulletLifetime = 10.f;

	float acceleration = 10.f;
	float rotationSteering = 220.f;
};


#endif //ASTEROIDS_SHIP_HPP
