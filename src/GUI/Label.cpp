#include <utility>
#include "Asteroids/GUI/Label.hpp"

#include "Asteroids/ServiceLocator/Locator.hpp"

Label::Label(std::string name): Widget(std::move(name))
{
	text.setFont(Locator::getResourceFont()->getFont());
	text.setCharacterSize(16);
	text.setFillColor(sf::Color::Black);
}

Label::Label(std::string name, std::string text): Label(std::move(name))
{
	setText(std::move(text));
}

void Label::handleEvent(const sf::Event& event)
{

}

void Label::update(float dt)
{
	if(textSource)
	{
		setText(textSource());
	}
}

void Label::draw(sf::RenderWindow& window)
{
	window.draw(text);
}

sf::Vector2f Label::getPosition()
{
	return text.getPosition();
}

sf::Vector2f Label::getSize()
{
	sf::FloatRect bounds = text.getLocalBounds();
	
	return {2 * bounds.left + bounds.width, 2 * bounds.top + bounds.height};
}

void Label::setPosition(sf::Vector2f position)
{
	text.setPosition(position);
}

void Label::recalculateSize()
{
	Widget::recalculateSize();
}

void Label::setText(std::string text)
{
	this->text.setString(text);
	recalculatePosition();
}

void Label::setColor(sf::Color color)
{
	text.setFillColor(color);
}

void Label::setTextSourceLambda(std::function<std::string()> func)
{
	textSource = std::move(func);
}