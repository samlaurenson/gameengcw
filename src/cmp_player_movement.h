#include "cmp_actor_movement.h"

class PlayerMovementComponent : public ActorMovementComponent {
public:
	explicit PlayerMovementComponent(Entity* p);
	PlayerMovementComponent() = delete;

	void update(double dt) override;
};