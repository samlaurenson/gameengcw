#include "ecm.h"
using namespace std;
using namespace sf;

const sf::Vector2f& Entity::getPosition() const { return _position; }

void Entity::setPosition(const Vector2f& pos) { _position = pos; }

Entity::Entity()
{
}

void Entity::update(const double dt) {
	for (auto& c : _components)
	{
		c->update(dt);
	}
}

void Entity::render(sf::RenderWindow& window)
{
	for (auto& c : _components)
	{
		c->render(window);
	}
}

void EntityManager::update(double dt)
{
	//Update every entity so long as the entity is alive
	for (auto& e : list)
	{
		if (e->isAlive())
		{
			e->update(dt);
		}
	}
}

void EntityManager::render(sf::RenderWindow& window)
{
	//Render every entity so long as that entity is alive
	for (auto& e : list)
	{
		if (e->isAlive())
		{
			e->render(window);
		}
	}
}

Component::Component(Entity* const p) : _parent(p)
{
}

/*
	#### GETTERS AND SETTERS ####
*/
#pragma region GetSetMethods
void Entity::setHealth(int health)
{
	_health = health;
}

int Entity::getHealth() const
{
	return _health;
}

void Entity::setFirerate(float fireRate)
{
	_fireRate = fireRate;
}

float Entity::getFirerate() const
{
	return _fireRate;
}

void Entity::setAlive(bool alive)
{
	_alive = alive;
}

bool Entity::isAlive() const
{
	return _alive;
}

void Entity::setHealthPool(int healthPool)
{
	_healthpool = healthPool;
}

int Entity::getHealthPool() const
{
	return _healthpool;
}

void Entity::setDamage(int damage)
{
	_damage = damage;
}

int Entity::getDamage() const
{
	return _damage;
}

void Entity::setRandomSpawnType(bool randomSpawn)
{
	_randomSpawn = randomSpawn;
}

bool Entity::getRandomSpawnType() const
{
	return _randomSpawn;
}
#pragma endregion