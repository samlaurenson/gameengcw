#include "bullet.h"
#include "game.h"
#include "sceneManager.h"
#include "cmp_actor_model.h"
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
		if (bullet._fired)
		{
			window.draw(bullet);
		}
	}
};

void Bullet::Fire(const sf::Vector2f& pos, const bool mode, sf::Vector2f direction, const int damage, const float range)
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
	b._range = range;

	//If player shoots - set bullet to player projectile style
	if (mode == false)
	{
		b.setTextureRect(sf::IntRect(0, 32, 32, 32));
	}

	if (mode == true)
	{
		b.setTextureRect(sf::IntRect(0, 32, 32, 32)); //change this to enemy bullet sprite
	}
};

void Bullet::_Update(const float& dt) {
	//If the bullet is not fired - then don't do anything. This is required to stop the LevelSystem compaining when it is trying to find the tile the bullet is on when it hasn't been shot yet
	if (!_fired)
	{
		return;
	}

	//Applying bullet range so after bullet has travelled a distance it will become inactive
	if (sf::length(startPoint - getPosition()) > _range)
	{
		_fired = false;
	}

	sf::Vector2f delta;

	//Getting difference between where to travel and the players position
	delta.x = direction.x - startPoint.x;
	delta.y = direction.y - startPoint.y;

	//Using inverse tan to find the angle between the player and the destination
	float angle = atan2(delta.y, delta.x);

	//Using cosine and sine functions to move the projectile to the destination
	move(cos(angle) * dt * 200.0f, sin(angle) * dt * 200.0f);

	//Will use this to check bullet collision for players and enemies
	std::vector<std::shared_ptr<Entity>> activeEntities = activeScene->getEnts();

	for (auto& e : activeEntities)
	{
		if (e->isAlive())
		{
			//sf::length(getPosition() - e->getPosition()) < 20.f
			float scalefac = e->GetCompatibleComponent<ActorModelComponent>()[0]->getScaleFactor();

			//Most likely a better way of bullet collision with scalable entities
			//Will get length between the bullet and the enemy position. If the bullet is less than 20 pixels multiplied by the scale factor of the enemy minus the scale factor multiplied by 2 (just to make bullet collide with enemy rather than disappear before hitting)
			//then bullet will hit the enemy
			if (sf::length(getPosition() - e->getPosition()) < (20.f * scalefac) - scalefac * 2)
			{
				//If player is hit by own bullet - do nothing
				if (!_mode && e == player)
				{
					continue;
				}

				//If enemy is hit by own bullet - do nothing
				if (_mode && e != player)
				{
					continue;
				}

				//Deal damage
				//Sets the entities health to their current health minus the damage of the bullet
				e->setHealth(e->getHealth() - _damage);

				std::cout << "Entity Health: " << e->getHealth() << std::endl;

				//After dealing damage, the bullet will be set to deal 0 damage and will be made invisible
				_damage = 0;
				_fired = false;
			}
		}
	}

	//If statements to provide better collision with walls
	if (ls::getTileAt(sf::Vector2f(getPosition().x - 16, getPosition().y)) == ls::WALL
		|| ls::getTileAt(sf::Vector2f(getPosition().x, getPosition().y + 16)) == ls::WALL
		|| ls::getTileAt(sf::Vector2f(getPosition().x + 16, getPosition().y)) == ls::WALL
		|| ls::getTileAt(sf::Vector2f(getPosition().x, getPosition().y - 16)) == ls::WALL)
	{
		//If touches wall - move off screen or hide bullet and set fired to false
		std::cout << "contact with wall" << std::endl;
		_fired = false;
	}
};

//function used to deactivate all bullets on screen 
void Bullet::clear()
{
	for (auto& bullet : bullets)
	{
		bullet._fired = false;
	}
}