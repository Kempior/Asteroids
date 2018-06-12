#include "Asteroids/GameObjects/Ship.hpp"

void Ship::Draw(sf::RenderWindow &window) {
	sprite.setPosition(position);
	sprite.setRotation(rotation);

	window.draw(sprite);
}
