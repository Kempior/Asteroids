#include <utility>
#include "Asteroids/GUI/Widget.hpp"

sf::Vector2u Widget::windowSize;

Widget::Widget(std::string widgetName):
name(std::move(widgetName))
{
	
}

Widget::~Widget()
{
	for(auto &childWidget : childWidgets)
	{
		delete childWidget;
	}
}

void Widget::initGUI(sf::RenderWindow& window)
{
	windowSize = window.getSize();
}

void Widget::handleEvent(const sf::Event& event)
{
	for(auto widget : childWidgets)
	{
		if(widget->isActive)
		{
			widget->handleEvent(event);
		}
	}
}

void Widget::update(float dt)
{
	for(auto widget : childWidgets)
	{
		if(widget->isActive)
		{
			widget->update(dt);
		}
	}
}

void Widget::draw(sf::RenderWindow& window)
{
	for(auto widget : childWidgets)
	{
		if(widget->isActive)
		{
			widget->draw(window);
		}
	}
}

void Widget::addChildWidget(Widget* widget)
{
	widget->parent = this;
	childWidgets.push_back(widget);
	widget->recalculateSize();
	widget->recalculatePosition();
	widget->updateStatus();
}

void Widget::recalculateSize()
{
	for(auto widget : childWidgets)
	{
		widget->recalculateSize();
	}
}

void Widget::recalculatePosition()
{
	if(parent == nullptr)
	{
		for(auto widget : childWidgets)
		{
			widget->recalculatePosition();
		}
		
		return;
	}
	
	sf::Vector2f newPosition;
	
	sf::Vector2f parentPos = parent->getPosition();
	sf::Vector2f parentSize = parent->getSize();
	
	newPosition = parentPos + sf::Vector2f(parentSize.x * anchor.x + anchorOffset.x, parentSize.y * anchor.y + anchorOffset.y);
	
	sf::Vector2f size = getSize();
	
	sf::Vector2f offset;
	offset.x += ((anchorMode & 8u) != 0 ? size.x : 0);
	offset.x += ((anchorMode & 4u) != 0 ? size.x / 2.0f : 0);
	
	offset.y += ((anchorMode & 2u) != 0 ? size.y : 0);
	offset.y += ((anchorMode & 1u) != 0 ? size.y / 2.0f : 0);
	
	newPosition -= offset;
	
	setPosition(newPosition);
	
	for(auto widget : childWidgets)
	{
		widget->recalculatePosition();
	}
}

void Widget::setAnchor(sf::Vector2f anchor, sf::Vector2f anchorOffset, AnchorPoint anchorMode)
{
	this->anchor = anchor;
	this->anchorOffset = anchorOffset;
	this->anchorMode = anchorMode;
	
	recalculatePosition();
}

void Widget::setIsActive(bool isActive)
{
	newIsActive = isActive;
	Widget* widget = this;
	while(widget)
	{
		widget->isModified = true;
		widget = widget->parent;
	}
}

void Widget::updateStatus()
{
	isModified = false;
	isActive = newIsActive;
	for(auto widget : childWidgets)
	{
		if(widget->isModified)
		{
			widget->updateStatus();
		}
	}
}
