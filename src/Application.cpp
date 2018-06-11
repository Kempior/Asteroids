#include "Asteroids/Application.hpp"

Application::Application()
{
	window.create(sf::VideoMode(1600, 900), "Asteroids");
}

void Application::run()
{
	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
	}
}