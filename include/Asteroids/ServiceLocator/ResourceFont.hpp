#ifndef ASTEROIDS_RESOURCEFONT_HPP
#define ASTEROIDS_RESOURCEFONT_HPP

#include "SFML/Graphics.hpp"

class ResourceFont
{
public:
	virtual ~ResourceFont() = default;
	
	virtual sf::Font& getFont() = 0;
};

#endif //ASTEROIDS_RESOURCEFONT_HPP