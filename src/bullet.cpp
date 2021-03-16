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

void Bullet::Fire(const sf::Vector2f& pos, const bool mode, sf::Vector2i direction)
{
	Bullet& b = bullets[++bulletPointer]; //getting memory address for object in bullets array
	b._mode = mode;
	b._fired = true;
	b.setPosition(pos);
	b.setTexture(spritesheet);
	b.direction = direction; //direction in which the bullet will travel e.g. from player to cursor

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
		delta.x = direction.x - player->getPosition().x;
		delta.y = direction.y - player->getPosition().y;

		//Using inverse tan to find the angle between the player and the destination
		float angle = atan2(delta.y, delta.x);

		//Using cosine and sine functions to move the projectile to the destination
		move(cos(angle) * dt * 200.0f, sin(angle) * dt * 200.0f);

		const sf::FloatRect boundingBox = getGlobalBounds();

		//Will use this to check bullet collision for players and enemies
		std::vector<std::shared_ptr<Entity>> activeEntities = activeScene->getEnts();

		//If the bullet is not fired - then don't do anything
		if (!_fired)
		{
			return;
		}

		if (ls::getTileAt(getPosition()) == ls::WALL)
		{
			//If touches wall - move off screen or hide bullet and set fired to false
			std::cout << "contact with wall" << std::endl;
			setPosition(400, 400);
		}
	}
};