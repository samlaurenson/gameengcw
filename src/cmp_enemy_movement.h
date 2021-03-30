#include "cmp_actor_movement.h"

class EnemyMovementComponent : public ActorMovementComponent {
protected:
	sf::Vector2f directionToTake;
public:
	explicit EnemyMovementComponent(Entity* p);
	EnemyMovementComponent() = delete;

	void setPath(sf::Vector2f start, sf::Vector2f finish);
	void update(double dt) override;
};