#include <utility>
#include "Asteroids/GUI/Box.hpp"

Box::Box(std::string name, sf::FloatRect rect): Widget(std::move(name)),
rect(rect)
{
	
}

void Box::handleEvent(const sf::Event& event)
{
	Widget::handleEvent(event);
}

void Box::update(float dt)
{
	Widget::update(dt);
}

void Box::draw(sf::RenderWindow& window)
{
	Widget::draw(window);
}

sf::Vector2f Box::getPosition()
{
	return position;
}

sf::Vector2f Box::getSize()
{
	return size;
}

void Box::setPosition(sf::Vector2f position)
{
	this->position = position;
}

void Box::recalculateSize()
{
	sf::Vector2f parentSize = parent->getSize();
	
	sf::Vector2f tl = sf::Vector2f(rect.left * parentSize.x, rect.top * parentSize.y);
	sf::Vector2f br = sf::Vector2f(rect.width * parentSize.x, rect.height * parentSize.y);
	
	size = br - tl;
	
	Widget::recalculateSize();
}