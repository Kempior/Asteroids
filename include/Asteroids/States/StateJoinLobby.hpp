#ifndef ASTEROIDS_STATEJOINLOBBY_HPP
#define ASTEROIDS_STATEJOINLOBBY_HPP

#include "Asteroids/States/State.hpp"

#include <string>

class StateJoinLobby : public State
{
public:
	StateJoinLobby();
	
	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;

private:
	std::string ip = "127.0.0.1";
};


#endif //ASTEROIDS_STATEJOINLOBBY_HPP