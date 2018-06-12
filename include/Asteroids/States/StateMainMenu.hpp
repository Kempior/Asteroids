#ifndef ASTEROIDS_STATEMAINMENU_HPP
#define ASTEROIDS_STATEMAINMENU_HPP

#include "Asteroids/States/State.hpp"

class StateMainMenu : public State
{
public:
	StateMainMenu();
	
	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;
};


#endif //ASTEROIDS_STATEMAINMENU_HPP