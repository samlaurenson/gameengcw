#pragma once
#include <../lib_ecm/ecm.h>

class ActorModelComponent : public Component
{
protected:
	sf::Sprite _sprite;
public:
	explicit ActorModelComponent(Entity* p);
	ActorModelComponent() = delete;

	void setModel(sf::IntRect model);

	void render(sf::RenderWindow& window) override;
	void update(double dt) override;
	void setScaleFactor(float scalefactor);
	float getScaleFactor() const;
};