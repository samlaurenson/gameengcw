#include "cmp_player_movement.h"

PlayerMovementComponent::PlayerMovementComponent(Entity* p) : ActorMovementComponent(p) {}

void PlayerMovementComponent::update(double dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		move(sf::Vector2f(0, -(_speed * dt)));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		move(sf::Vector2f(0, (_speed * dt)));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		move(sf::Vector2f(-(_speed * dt), 0));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		move(sf::Vector2f((_speed * dt), 0));
	}

	ActorMovementComponent::update(dt);
}