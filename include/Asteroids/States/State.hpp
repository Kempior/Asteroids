#ifndef ASTEROIDS_STATE_HPP
#define ASTEROIDS_STATE_HPP

#include "Asteroids/GUI/Canvas.hpp"

#include "SFML/Graphics.hpp"

class State;

struct ReturnState
{
	State* state = nullptr;
	bool destroyLast = false;
};

class State
{
public:
	virtual ~State() = default;
	
	virtual void handleEvent(const sf::Event& event) = 0;
	virtual void update(float dt) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
	
	static ReturnState currentState;
	
public:
	sf::Color clearColor = sf::Color::White;

protected:
	Canvas canvas;
};

#endif //ASTEROIDS_STATE_HPP