#include "Asteroids/Application.hpp"

#include "Asteroids/States/StateGame.hpp"
#include "Asteroids/States/StateMainMenu.hpp"
#include "Asteroids/GUI/Widget.hpp"
#include "Asteroids/ServiceLocator/Locator.hpp"
#include "Asteroids/ServiceLocator/DefaultResourceFont.hpp"

#include <chrono>
#include <thread>

Application::Application()
{
	window.create(sf::VideoMode(1600, 900), "Asteroids");
	
	Widget::initGUI(window);
	Locator::provideFont(new DefaultResourceFont());
	
	//currentState = new StateGame();
	currentState = new StateMainMenu();
	State::currentState.state = currentState;
}

void Application::run()
{
	sf::Clock clock, frameClock;
	sf::Time frameElapsedTime, frameTime = sf::seconds(1.0f / 60.0f);
	
	while(window.isOpen() && currentState)
	{
		sf::Event event{};
		while(window.pollEvent(event))
		{
			currentState->handleEvent(event);
			
			if(event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		
		currentState->update(clock.restart().asSeconds());
		
		window.clear(currentState->clearColor);
		
		currentState->draw(window);
		
		window.display();
		
		if(State::currentState.destroyLast)
		{
			State::currentState.destroyLast = false;
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