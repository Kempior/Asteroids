#include "Asteroids/States/State.hpp"

ReturnState State::currentState;

void State::handleEvent(const sf::Event& event)
{
	canvas.handleEvent(event);
}

void State::update(float dt)
{
	canvas.update(dt);
}

void State::draw(sf::RenderWindow& window)
{
	canvas.draw(window);
}