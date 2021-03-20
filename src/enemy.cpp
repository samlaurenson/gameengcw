#include "enemy.h"
#include "sceneManager.h"

Enemy::Enemy() {}

void Enemy::update(double dt)
{
	if (sf::length(player->getPosition() - getPosition()) < _detectionDistance)
	{
		std::cout << "Player detected by enemy" << std::endl;
	}

	Entity::update(dt);
}

void Enemy::render(sf::RenderWindow& window)
{
	Entity::render(window);
}

void Enemy::addBuffDrop(std::shared_ptr<Component> component)
{
	_buffDrop = component;
}

void Enemy::setDetectionDistance(float distance)
{
	_detectionDistance = distance;
}

float Enemy::getDetectionDistance() const
{
	return _detectionDistance;
}