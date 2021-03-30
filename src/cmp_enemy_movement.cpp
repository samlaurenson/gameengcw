#include "cmp_enemy_movement.h"

EnemyMovementComponent::EnemyMovementComponent(Entity* p) : ActorMovementComponent(p) {}

void EnemyMovementComponent::update(double dt)
{
	//A* pathfinding was too expensive to use since our enemies will always be following the player
	//after being seen - so enemies will now just start moving towards the player after the player has been seen

	//If the player has been seen - then chase the enemy
	move(directionToTake.x * _speed * dt, directionToTake.y * _speed * dt);
}

void EnemyMovementComponent::setPath(sf::Vector2f start, sf::Vector2f finish)
{
	sf::Vector2f difference = finish - start;

	//normalising the vector to obtain the direction that the enemy should move in
	//to move towards the player
	directionToTake = sf::normalize(difference);
}