#include "Asteroids/GameObjects/GameObject.hpp"

bool GameObject::Collide(GameObject &other) {
	sf::Vector2f diff = position - other.position;

	return sqrtf(diff.x * diff.x + diff.y * diff.y) <= radius + other.radius;
}