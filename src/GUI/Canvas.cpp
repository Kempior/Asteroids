#include "Asteroids/GUI/Canvas.hpp"

Canvas::Canvas(sf::RenderWindow& window): Widget("Canvas")
{
	windowSize = window.getSize();
}

void Canvas::handleEvent(const sf::Event& event)
{
	if(event.type == sf::Event::Resized)
	{
		windowSize.x = event.size.width;
		windowSize.y = event.size.height;

		recalculateSize();
		recalculatePosition();
	}
	
	Widget::handleEvent(event);
}

void Canvas::update(float dt)
{
	updateStatus();
	
	Widget::update(dt);
}

void Canvas::draw(sf::RenderWindow& window)
{
	Widget::draw(window);
}

sf::Vector2f Canvas::getPosition()
{
	return {};
}

sf::Vector2f Canvas::getSize()
{
	return sf::Vector2f(windowSize);
}

void Canvas::recalculateSize()
{
	Widget::recalculateSize();
}