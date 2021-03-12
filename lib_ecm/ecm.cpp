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
	for (auto& e : list)
	{
		e->update(dt);
	}
}

void EntityManager::render(sf::RenderWindow& window)
{
	for (auto& e : list)
	{
		e->render(window);
	}
}

Component::Component(Entity* const p) : _parent(p)
{
}
