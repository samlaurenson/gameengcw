#include "cmp_player_movement.h"
#include "bullet.h"
#include "game.h"

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

	//Player control for shooting
	if (_parent->isAlive())
	{
		//Setting player to dead if they have no health
		if (_parent->getHealth() <= 0)
		{
			_parent->setAlive(false);
			return;
		}

		static float firetime = 1.0f;
		firetime -= dt;

		//Should probably put this in a better place but will work here for now since a RenderWindow is required
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && firetime <= 0)
		{
			firetime = _parent->getFirerate();
			Bullet::Fire(_parent->getPosition(), false, mousepos, _parent->getDamage(), _parent->getBulletRange());
		}
	}

	ActorMovementComponent::update(dt);
}