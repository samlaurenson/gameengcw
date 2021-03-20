#pragma once
#include "../lib_ecm/ecm.h"

class ActorBuffComponent : public Component
{
public:
	explicit ActorBuffComponent(Entity* p);
	ActorBuffComponent() = delete;

	void update(double dt) override;
	void render(sf::RenderWindow& window) override;

	void applyBuff(std::shared_ptr<Buff> buff) const;
};