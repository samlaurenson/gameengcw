#include "enemy.h"
#include "bullet.h"
#include "sceneManager.h"
#include "cmp_actor_buff.h"
#include "cmp_enemy_movement.h"

Enemy::Enemy() {}

void Enemy::update(double dt)
{
	//If enemy and player are alive and the player is within the enemy detection distance, then enemy will shoot at player
	//Add in if enemy health is less than their health pool (they have taken damage), then start walking towards player and once in range then shoot
	if ((sf::length(player->getPosition() - getPosition()) < _detectionDistance) && isAlive() && player->isAlive())
	{
		_playerSeen = true; //If player enters enemy detection zone - then enemy will be marked as having seen the player

		//If the enemy can move
		if (get_components<EnemyMovementComponent>().size() != 0)
		{
			GetCompatibleComponent<EnemyMovementComponent>()[0]->setPath(getPosition(), player->getPosition());
		}

		static float firetime = 0.0f;
		firetime -= dt;
		if (firetime <= 0)
		{
			firetime = getFirerate();
			Bullet::Fire(getPosition(), true, player->getPosition(), getDamage(), getBulletRange());

		}
	}
	else {
		//Temporary measure to stop enemies from carrying on seen behaviour after game reset
		_playerSeen = false;
		if (get_components<EnemyMovementComponent>().size() != 0)
		{
			GetCompatibleComponent<EnemyMovementComponent>()[0]->setPath(sf::Vector2f(0,0), sf::Vector2f(0, 0));
		}
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

void Entity::setBulletRange(float range)
{
	_bulletRange = range;
}

float Entity::getBulletRange() const
{
	return _bulletRange;
}