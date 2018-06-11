#ifndef ASTEROIDS_STATE_HPP
#define ASTEROIDS_STATE_HPP

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
	State();
	virtual ~State();
	
	virtual void handleEvent(const sf::Event& event) = 0;
	virtual void update(float dt) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
	
	static ReturnState currentState;
};

#endif //ASTEROIDS_STATE_HPP