#pragma once
#include "../lib_ecm/ecm.h"

class Enemy : public Entity
{
protected:
	float _detectionDistance;
	std::shared_ptr<Component> _buffDrop;
public:
	Enemy();
	virtual ~Enemy() = default;
	void update(double dt) override;
	void render(sf::RenderWindow& window) override;

	void addBuffDrop(std::shared_ptr<Component> buff);
	void setDetectionDistance(float _detectionDistance);
	float getDetectionDistance() const;
};