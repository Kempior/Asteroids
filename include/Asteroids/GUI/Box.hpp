#ifndef ASTEROIDS_BOX_HPP
#define ASTEROIDS_BOX_HPP

#include "Asteroids/GUI/Widget.hpp"

#include <string>

class Box : public Widget
{
public:
	Box(std::string name, sf::FloatRect rect);
	
	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;
	
	sf::Vector2f getPosition() override;
	sf::Vector2f getSize() override;
	void setPosition(sf::Vector2f position) override;
	
	void recalculateSize() override;
	
private:
	sf::Vector2f position;
	sf::Vector2f size;
	
	sf::FloatRect rect;
};

#endif //ASTEROIDS_BOX_HPP