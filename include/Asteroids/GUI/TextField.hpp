#ifndef ASTEROIDS_TEXTFIELD_HPP
#define ASTEROIDS_TEXTFIELD_HPP

#include "Asteroids/GUI/Widget.hpp"

#include <string>
#include <functional>

class Label;

class TextField : public Widget
{
public:
	TextField(std::string name, sf::Vector2f size);
	
	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;
	
	sf::Vector2f getPosition() override;
	sf::Vector2f getSize() override;
	void setPosition(sf::Vector2f position) override;
	void setSize(sf::Vector2f size);
	
	void recalculateSize() override;
	
	void setValidateFunc(std::function<bool(std::string)> validateFunc);
	void setTextSourceFunc(std::function<std::string()> textSourceFunc);
	void setDataUpdateFunc(std::function<void()> dataUpdateFunc);
	void setText(std::string str);
	std::string getText();

private:
	sf::RectangleShape textFieldRect;
	Label* text;
	
	std::function<bool(std::string)> validateFunc;
	std::function<std::string()> textSourceFunc;
	std::function<void()> dataUpdateFunc;
	bool isSelected = false;
	std::string inputedText, oldText;
};

#endif //ASTEROIDS_TEXTFIELD_HPP