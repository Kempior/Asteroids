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
    // In degrees
	float rotation = 180.f;
	// In degrees per second
	float rotationSpeed = 0.f;
	float radius = 0.f;

	//float scale = 1;

	bool isDestroyed = false;

    void Draw(sf::RenderWindow &window);
    bool Collide(GameObject &other, float padding = 0.f) const;
	sf::Vector2f Forward() const;

	void Constrain(sf::IntRect, float dt);
};


#endif //ASTEROIDS_GAMEOBJECT_HPP
