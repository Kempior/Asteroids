#include "Asteroids/GameObjects/GameObject.hpp"

void GameObject::Draw(sf::RenderWindow &window) {
	if (isDestroyed)
		return;

	sprite.setPosition(position);
	sprite.setRotation(rotation + 180.f);

	window.draw(sprite);
}

bool GameObject::Collide(GameObject &other) const {
	sf::Vector2f diff = position - other.position;

	return sqrtf(diff.x * diff.x + diff.y * diff.y) <= radius + other.radius;
}

sf::Vector2f GameObject::Forward() const {

	sf::Vector2f returnVec(sinf(-rotation * (float)M_PI / 180), cosf(-rotation * (float)M_PI / 180));

	return returnVec;
}

void GameObject::Constrain(sf::IntRect rect, float dt) {
	//Constraints on x axis
	if (position.x < rect.left) {
		velocity.x *= -1;
		position.x += velocity.x * dt;
	}
	else if (rect.left + rect.width < position.x) {
		velocity.x *= -1;
		position.x += velocity.x * dt;
	}
	//Constraints on y axis
	if (position.y < rect.top) {
		velocity.y *= -1;
		position.y += velocity.y * dt;
	}
	else if (rect.top + rect.height < position.y) {
		velocity.y *= -1;
		position.y += velocity.y * dt;
	}
}
