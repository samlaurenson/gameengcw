#include "cmp_actor_movement.h"

using namespace sf;

void ActorMovementComponent::update(double dt) {  }

ActorMovementComponent::ActorMovementComponent(Entity* p) : _speed(150.0f), Component(p) {}

//Function to check if there is an impassable object in the way of where the player will be moving
bool ActorMovementComponent::validMove(const sf::Vector2f& pos) {
	return (LevelSystem::getTileAt(pos) != LevelSystem::WALL);
}

void ActorMovementComponent::move(const sf::Vector2f& p)
{
	// pp = player position
	auto pp = _parent->getPosition() + p;
	if (validMove(pp))
	{
		_parent->setPosition(pp);
	}
}

void ActorMovementComponent::move(float x, float y) {
	move(Vector2f(x, y));
}

float ActorMovementComponent::getSpeed() const { return _speed; }

void ActorMovementComponent::setSpeed(float speed) { _speed = speed; }