#include "Asteroids/ServiceLocator/Locator.hpp"

ResourceFont* Locator::fontService = nullptr;

void Locator::terminate()
{
	delete fontService;
}

ResourceFont* Locator::getResourceFont()
{
	return fontService;
}

void Locator::provideFont(ResourceFont* fontService)
{
	delete Locator::fontService;
	Locator::fontService = fontService;
}