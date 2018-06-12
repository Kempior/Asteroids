#include <cmath>

#include "Asteroids/GameObjects/Asteroid.hpp"

void Asteroid::Draw(sf::RenderWindow &window) {
	sprite.setRotation(rotation);
	sprite.setPosition(position);

	window.draw(sprite);
}
