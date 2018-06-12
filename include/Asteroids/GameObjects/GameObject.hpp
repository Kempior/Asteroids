#ifndef ASTEROIDS_GAMEOBJECT_HPP
#define ASTEROIDS_GAMEOBJECT_HPP

#include <string>

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

class GameObject {
public:
	explicit GameObject(sf::Vector2f position = sf::Vector2f(), sf::Vector2f velocity = sf::Vector2f(), float rotation = 180.f, float rotationSpeed = 0.f)
			: position(position), velocity(velocity), rotation(rotation), rotationSpeed(rotationSpeed) {}
    virtual ~GameObject() = default;

	sf::Sprite sprite;

    sf::Vector2f position;
    sf::Vector2f velocity;
	float rotation = 180.f;
	float rotationSpeed = 0.f;
	float radius = 1.f;

    void Draw(sf::RenderWindow &window);
    bool Collide(GameObject &other) const;
	sf::Vector2f Forward() const;
};


#endif //ASTEROIDS_GAMEOBJECT_HPP
