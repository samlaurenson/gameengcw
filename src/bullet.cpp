#include "bullet.h"
#include "game.h"
#include "sceneManager.h"
#include <LevelSystem.h>
#include <iostream>

unsigned char Bullet::bulletPointer = 0;
Bullet Bullet::bullets[256];

Bullet::Bullet() : Sprite() {
	_mode = false;
	_fired = false;
};

void Bullet::Update(const float& dt)
{
	for (auto& bullet : bullets)
	{
		bullet._Update(dt);
	}
};

void Bullet::Render(sf::RenderWindow& window)
{
	for (auto& bullet : bullets)
	{
		window.draw(bullet);
	}
};

void Bullet::Fire(const sf::Vector2f& pos, const bool mode, sf::Vector2i direction, int damage)
{
	Bullet& b = bullets[++bulletPointer]; //getting memory address for object in bullets array
	b._mode = mode;
	b._fired = true;
	b._damage = damage;
	b.setPosition(pos);		 //Setting position to players position when firing bullet
	b.setTexture(spritesheet);
	b.setOrigin(16.f, 16.f);
	b.startPoint = pos;		 //Position of where the bullet is fired from
	b.direction = direction; //direction in which the bullet will travel e.g. from startPoint to cursor

	//If player shoots - set bullet to player projectile style
	if (mode == false)
	{
		b.setTextureRect(sf::IntRect(0, 0, 32, 32));
	}
};

void Bullet::_Update(const float& dt) {

	if (getPosition().y < -32 || getPosition().y > 1080 + 32 || getPosition().x > 1920 + 32 || getPosition().x < -32)
	{
		//off screen - do nothing
		return;
	}
	else {
		//Keeping it simple for now -- simply just moving the bullets
		sf::Vector2f delta;

		//Getting difference between where to travel and the players position
		delta.x = direction.x - startPoint.x;
		delta.y = direction.y - startPoint.y;

		//Using inverse tan to find the angle between the player and the destination
		float angle = atan2(delta.y, delta.x);

		//Using cosine and sine functions to move the projectile to the destination
		move(cos(angle) * dt * 200.0f, sin(angle) * dt * 200.0f);

		const sf::FloatRect boundingBox = getGlobalBounds(); //Maybe would be better to do collisions like how they are done in the pacman lab between ghosts?

		//Will use this to check bullet collision for players and enemies
		std::vector<std::shared_ptr<Entity>> activeEntities = activeScene->getEnts();


		//sf::length(player->getPosition() - e->getPosition()) < 30.f)
		for (auto& e : activeEntities)
		{
			if (sf::length(getPosition() - e->getPosition()) < 15.f)
			{
				//If player is hit by own bullet - do nothing
				if (!_mode && e == player)
				{
					std::cout << "Player hit by own bullet" << std::endl;
					continue;
				}

				//If enemy is hit by own bullet - do nothing
				if (_mode && e != player)
				{
					std::cout << "Enemy hit by own bullet" << std::endl;
					continue;
				}

				//Deal damage
				//Sets the entities health to their current health minus the damage of the bullet
				e->setHealth(e->getHealth() - _damage);

				std::cout << "Enemy Health: " << e->getHealth() << std::endl;
				//Now get rid of bullet to prevent bullet from dealing damage as it travels through the enemy
				_damage = 0;
				
				setTextureRect(sf::IntRect(0, 0, 0, 0));

				//If entity is dead
				if (e->getHealth() <= 0)
				{
					std::cout << "Enemy is dead" << std::endl;
				}
			}
		}

		//If the bullet is not fired - then don't do anything
		if (!_fired)
		{
			return;
		}

		//If statements to provide better collision with walls
		if (ls::getTileAt(sf::Vector2f(getPosition().x-16, getPosition().y)) == ls::WALL
			|| ls::getTileAt(sf::Vector2f(getPosition().x, getPosition().y+16)) == ls::WALL 
			|| ls::getTileAt(sf::Vector2f(getPosition().x+16, getPosition().y)) == ls::WALL 
			|| ls::getTileAt(sf::Vector2f(getPosition().x, getPosition().y-16)) == ls::WALL)
		{
			//If touches wall - move off screen or hide bullet and set fired to false
			std::cout << "contact with wall" << std::endl;
			setPosition(400, 400);
		}
	}
};