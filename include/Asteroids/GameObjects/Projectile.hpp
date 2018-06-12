#ifndef ASTEROIDS_PROJECTILE_HPP
#define ASTEROIDS_PROJECTILE_HPP

#include "Asteroids/GameObjects/GameObject.hpp"

class Projectile : public GameObject {
public:
	explicit Projectile(int ownerID, sf::Vector2f position = sf::Vector2f(), sf::Vector2f velocity = sf::Vector2f(), float rotation = 180.f, float rotationSpeed = 0.f)
			: GameObject(position, velocity, rotation, rotationSpeed) {}
};


#endif //ASTEROIDS_PROJECTILE_HPP
