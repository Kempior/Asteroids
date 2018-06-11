#ifndef ASTEROIDS_STATEGAME_HPP
#define ASTEROIDS_STATEGAME_HPP

#include "Asteroids/States/State.hpp"

class StateGame : public State
{
public:
	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;
};

#endif //ASTEROIDS_STATEGAME_HPP