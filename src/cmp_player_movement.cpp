#include "cmp_player_movement.h"
#include "bullet.h"

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

	//Not a movement but will be here to keep controls centralised 
	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		Bullet::Fire(_parent->getPosition(), false, );
	}*/

	/*if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2i mousepos = sf::Mouse::getPosition(window);
		Bullet::Fire(_parent->getPosition(), false, mousepos.x, mousepos.y);
		firetime = 0.7f;
	}*/

	ActorMovementComponent::update(dt);
}