#ifndef ASTEROIDS_CANVAS_HPP
#define ASTEROIDS_CANVAS_HPP

#include "Asteroids/GUI/Widget.hpp"

class Canvas : public Widget
{
public:
	Canvas();

	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;
	
	sf::Vector2f getPosition() override;
	sf::Vector2f getSize() override;
	void setPosition(sf::Vector2f position) override;
	
	void recalculateSize() override;
};

#endif //ASTEROIDS_CANVAS_HPP