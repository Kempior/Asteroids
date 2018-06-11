#include "Asteroids/Application.hpp"

#include "Asteroids/States/StateGame.hpp"

#include <chrono>
#include <thread>

Application::Application()
{
	window.create(sf::VideoMode(1600, 900), "Asteroids");
	
	currentState = new StateGame();
	State::currentState.state = currentState;
}

void Application::run()
{
	sf::Clock clock, frameClock;
	sf::Time frameElapsedTime, frameTime = sf::seconds(1.0f / 60.0f);
	
	while(window.isOpen() && currentState)
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			currentState->handleEvent(event);
			
			if(event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		
		currentState->update(clock.restart().asSeconds());
		
		currentState->draw(window);
		
		if(State::currentState.destroyLast)
		{
			delete currentState;
		}
		currentState = State::currentState.state;
		
		frameElapsedTime += frameClock.restart();
		if(frameElapsedTime >= frameTime)
		{
			frameElapsedTime -= frameTime;
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds((frameTime - frameElapsedTime).asMilliseconds()));
			frameElapsedTime = sf::Time::Zero;
		}
	}
}