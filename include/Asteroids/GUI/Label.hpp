#ifndef ASTEROIDS_LABEL_HPP
#define ASTEROIDS_LABEL_HPP

#include "Asteroids/GUI/Widget.hpp"

#include <string>
#include <functional>

class Label : public Widget
{
public:
	explicit Label(std::string name);
	Label(std::string name, std::string text);
	
	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;
	
	sf::Vector2f getPosition() override;
	sf::Vector2f getSize() override;
	void setPosition(sf::Vector2f position) override;
	
	void recalculateSize() override;
	
	void setText(std::string text);
	void setColor(sf::Color color);
	void setTextSourceLambda(std::function<std::string()> func);
	
private:
	sf::Text text;
	std::function<std::string()> textSource;
};

#endif //ASTEROIDS_LABEL_HPP