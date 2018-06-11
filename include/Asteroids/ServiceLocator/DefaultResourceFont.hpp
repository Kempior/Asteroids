#ifndef ASTEROIDS_DEFAULTRESOURCEFONT_HPP
#define ASTEROIDS_DEFAULTRESOURCEFONT_HPP

#include "Asteroids/ServiceLocator/ResourceFont.hpp"

class DefaultResourceFont : public ResourceFont
{
public:
	DefaultResourceFont();
	
	sf::Font& getFont() override;
	
private:
	sf::Font font;
};

#endif //ASTEROIDS_DEFAULTRESOURCEFONT_HPP