#ifndef ASTEROIDS_APPLICATION_HPP
#define ASTEROIDS_APPLICATION_HPP

#include "SFML/Graphics.hpp"

class State;

class Application
{
public:
	Application();
	
	void run();

private:
	sf::RenderWindow window;
	State *currentState;
};

#endif //ASTEROIDS_APPLICATION_HPP