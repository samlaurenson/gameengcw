#include "enemy.h"
#include "bullet.h"
#include "sceneManager.h"
#include "cmp_actor_buff.h"

Enemy::Enemy() {}

void Enemy::update(double dt)
{
	//If enemy and player are alive and the player is within the enemy detection distance, then enemy will shoot at player
	if (sf::length(player->getPosition() - getPosition()) < _detectionDistance && isAlive() && player->isAlive())
	{
		static float firetime = 0.0f;
		firetime -= dt;
		if (firetime <= 0)
		{
			firetime = getFirerate();
			Bullet::Fire(getPosition(), true, player->getPosition(), 25);

		}
		std::cout << "Player detected by enemy" << std::endl;
	}

	//Applying buff to player upon enemy death
	if (getHealth() <= 0 && isAlive())
	{
		setAlive(false);
		player->GetCompatibleComponent<ActorBuffComponent>()[0]->applyBuff(_buffDrop);
	}

	Entity::update(dt);
}

void Enemy::render(sf::RenderWindow& window)
{
	Entity::render(window);
}

void Enemy::addBuffDrop(std::shared_ptr<Buff> drop)
{
	_buffDrop = drop;
}

void Enemy::setDetectionDistance(float distance)
{
	_detectionDistance = distance;
}

float Enemy::getDetectionDistance() const
{
	return _detectionDistance;
}