#include <utility>
#include "Asteroids/GUI/Button.hpp"

Button::Button(std::string name, sf::Vector2f size): Widget(std::move(name))
{
	setSize(size);
	buttonRect.setFillColor(sf::Color(34, 145, 32, 255));
}

Button::Button(std::string name, sf::Vector2f size, std::function<void()> func): Button(std::move(name), size)
{
	funcCallback = std::move(func);
}

void Button::handleEvent(const sf::Event& event)
{
	if(event.type == sf::Event::MouseButtonPressed  && event.mouseButton.button == sf::Mouse::Left)
	{
		if(	event.mouseButton.x < buttonRect.getPosition().x ||
			event.mouseButton.x > buttonRect.getPosition().x + buttonRect.getSize().x ||
			event.mouseButton.y < buttonRect.getPosition().y ||
			event.mouseButton.y > buttonRect.getPosition().y + buttonRect.getSize().y)
		{
			return;
		}
		
		funcCallback();
	}
	
	Widget::handleEvent(event);
}

void Button::update(float dt)
{
	Widget::update(dt);
}

void Button::draw(sf::RenderWindow& window)
{
	window.draw(buttonRect);
	
	Widget::draw(window);
}

sf::Vector2f Button::getPosition()
{
	return buttonRect.getPosition();
}

sf::Vector2f Button::getSize()
{
	return buttonRect.getSize();
}

void Button::setPosition(sf::Vector2f position)
{
	buttonRect.setPosition(position);
}

void Button::setSize(sf::Vector2f size)
{
	buttonRect.setSize(size);
	recalculatePosition();
}

void Button::recalculateSize()
{
	Widget::recalculateSize();
}

void Button::setFunc(std::function<void()> func)
{
	funcCallback = std::move(func);
}