#include "Asteroids/ServiceLocator/DefaultResourceFont.hpp"

DefaultResourceFont::DefaultResourceFont()
{
	font.loadFromFile("Font.ttf");
}

sf::Font& DefaultResourceFont::getFont()
{
	return font;
}