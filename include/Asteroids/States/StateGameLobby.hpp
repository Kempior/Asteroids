#ifndef ASTEROIDS_STATEGAMELOBBY_HPP
#define ASTEROIDS_STATEGAMELOBBY_HPP

#include "Asteroids/States/State.hpp"

namespace sf
{
	class TcpListener;
	class TcpSocket;
}

class StateGameLobby : public State
{
public:
	explicit StateGameLobby(bool isHost);
	explicit StateGameLobby(sf::TcpSocket* socket);
	~StateGameLobby() override ;
	
	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;

private:
	bool isHost;
	sf::TcpListener *listener = nullptr;
	sf::TcpSocket *server = nullptr;
	std::vector<sf::TcpSocket*> clients;
	
	unsigned int playerCount = 0;
};


#endif //ASTEROIDS_STATEGAMELOBBY_HPP