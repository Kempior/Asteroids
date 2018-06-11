#ifndef ASTEROIDS_LOCATOR_HPP
#define ASTEROIDS_LOCATOR_HPP

#include "Asteroids/ServiceLocator/ResourceFont.hpp"

class Locator
{
public:
	static void terminate();

	static ResourceFont* getResourceFont();
	static void provideFont(ResourceFont* fontService);
	
private:
	static ResourceFont* fontService;
};

#endif //ASTEROIDS_LOCATOR_HPP