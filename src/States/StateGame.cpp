#include <random>
#include <ctime>

#include "SFML/Window.hpp"

#include "Asteroids/States/StateGame.hpp"

StateGame::StateGame(int playerCount, int playerID, sf::IntRect worldSize) : worldSize(worldSize), playerID(playerID) {

	atlasTexture.loadFromFile("../../assets/Atlas.png");

	clearColor = sf::Color::Black;

	CreateAsteroids(20);
	for (int i = 0; i < playerCount; ++i) {
		CreateShip({worldSize.left + (worldSize.width / (float)(playerCount + 1)) * (i + 1), worldSize.top + worldSize.height * 0.75f});
	}
}

void StateGame::CreateAsteroids(int howMany) {

	std::mt19937 mt(time(NULL));
	std::uniform_real_distribution<float> xPos(worldSize.left, worldSize.left + worldSize.width);
	std::uniform_real_distribution<float> yPos(worldSize.top, worldSize.top + worldSize.height);

	std::uniform_real_distribution<float> spd(-50, 50);
	std::uniform_real_distribution<float> rot(0, 360.f);

	std::uniform_real_distribution<float> rotSpd(-30.f, 30.f);

	for (int i = 0; i < howMany; ++i) {
		Asteroid newAst ({xPos(mt), yPos(mt)}, {spd(mt), spd(mt)});
		newAst.sprite.setTextureRect({0, 0, 128, 128});
		newAst.sprite.setOrigin(63, 63);

		newAst.rotation = rot(mt);
		newAst.rotationSpeed = rotSpd(mt);

		newAst.sprite.setTexture(atlasTexture);

		newAst.sprite.scale({1.f, 1.f});
		newAst.radius = 64;

		asteroids.push_back(newAst);
	}
}

void StateGame::CreateAsteroids(int howMany, sf::Vector2f position) {

	std::mt19937 mt(time(NULL));

	std::uniform_real_distribution<float> spd(-50, 50);
	std::uniform_real_distribution<float> rot(0, 360.f);

	std::uniform_real_distribution<float> rotSpd(-30.f, 30.f);

	for (int i = 0; i < howMany; ++i) {
		Asteroid newAst (position);

		newAst.sprite.setTextureRect({0, 0, 128, 128});
		newAst.sprite.setOrigin(63, 63);

		newAst.rotation = rot(mt);
		newAst.rotationSpeed = rotSpd(mt);

		newAst.sprite.setTexture(atlasTexture);

		asteroids.push_back(newAst);
	}
}

void StateGame::CreateShip(sf::Vector2f position) {

	Ship newShip(position);

	newShip.sprite.setTextureRect({128, 0, 128, 128});
	newShip.sprite.setOrigin(63, 63);

	newShip.sprite.setTexture(atlasTexture);

	newShip.sprite.scale({0.5, 0.5});
	newShip.radius = 32;

	ships.push_back(newShip);
}

void StateGame::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
			case sf::Keyboard::W:
				ships[playerID].isAccelerating = true;
				break;
			case sf::Keyboard::A:
				ships[playerID].isRotatingLeft = true;
				break;
			case sf::Keyboard::D:
				ships[playerID].isRotatingRight = true;
				break;
			default:
				break;
		}
	}
	else if (event.type == sf::Event::KeyReleased) {
		switch (event.key.code) {
			case sf::Keyboard::W:
				ships[playerID].isAccelerating = false;
				break;
			case sf::Keyboard::A:
				ships[playerID].isRotatingLeft = false;
				break;
			case sf::Keyboard::D:
				ships[playerID].isRotatingRight = false;
				break;
			case sf::Keyboard::Space:
				ships[playerID].isShooting = false;
				break;
			default:
				break;
		}
	}
}

void StateGame::update(float dt)
{
	for (auto &roid : asteroids) {
		roid.position += roid.velocity * dt;
		roid.rotation += roid.rotationSpeed * dt;

		roid.Constrain(worldSize, dt);
	}
	for (auto &ship : ships) {
		if (ship.isAccelerating) {
			ship.velocity += ship.acceleration * ship.Forward();
		}

		if (ship.isRotatingLeft && !ship.isRotatingRight) {
			ship.rotationSpeed = -ship.rotationSteering;
		}
		else if (!ship.isRotatingLeft && ship.isRotatingRight) {
			ship.rotationSpeed = ship.rotationSteering;
		}
		else {
			ship.rotationSpeed = 0.f;
		}

		if (ship.isShooting) {

		}

		ship.position += ship.velocity * dt;
		ship.rotation += ship.rotationSpeed * dt;

		ship.Constrain(worldSize, dt);
	}
}

void StateGame::draw(sf::RenderWindow& window)
{
	for (auto &roid : asteroids) {
		roid.Draw(window);
	}
	for (auto &ship : ships) {
		ship.Draw(window);
	}
}

