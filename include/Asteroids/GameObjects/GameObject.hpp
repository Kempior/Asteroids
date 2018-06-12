#ifndef ASTEROIDS_GAMEOBJECT_HPP
#define ASTEROIDS_GAMEOBJECT_HPP

#include <string>

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

class GameObject {
public:
	explicit GameObject(sf::Vector2f position = sf::Vector2f(), sf::Vector2f velocity = sf::Vector2f(), float rotation = 0.f, float rotationSpeed = 0.f)
			: position(position), velocity(velocity), rotation(rotation), rotationSpeed(rotationSpeed) {}
    virtual ~GameObject() = default;

	sf::Sprite sprite;

    sf::Vector2f position;
    sf::Vector2f velocity;
	float rotation = 0;
	float rotationSpeed = 0;
	float radius = 1.f;

    virtual void Draw(sf::RenderWindow &window) = 0;
    virtual bool Collide(GameObject &other);
};


#endif //ASTEROIDS_GAMEOBJECT_HPP
