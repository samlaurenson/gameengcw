#pragma once
#include <../lib_ecm/ecm.h>
#include <LevelSystem.h>

//Component that will be used to set the speed of entities and to determine if a move is valid
class ActorMovementComponent : public Component {
protected:
	bool validMove(const sf::Vector2f&);
	float _speed;

public:
	explicit ActorMovementComponent(Entity* p);
	ActorMovementComponent() = delete;

	float getSpeed() const;
	void setSpeed(float _speed);

	void move(const sf::Vector2f&);
	void move(float x, float y);

	void render(sf::RenderWindow& window) override {}
	void update(double dt) override;
};