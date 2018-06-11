#ifndef ASTEROIDS_APPLICATION_HPP
#define ASTEROIDS_APPLICATION_HPP

#include "SFML/Graphics.hpp"

class Application
{
public:
	Application();
	
	void run();

private:
	sf::RenderWindow window;
};

#endif //ASTEROIDS_APPLICATION_HPP