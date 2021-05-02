#include "cmp_actor_model.h"
#include "game.h"

//Compnent that allows the entity to have a model set to them
ActorModelComponent::ActorModelComponent(Entity* p) : Component(p) {}

void ActorModelComponent::setModel(sf::IntRect model)
{
	_sprite.setTexture(spritesheet);
	_sprite.setTextureRect(model);
	_sprite.setOrigin(16.f, 16.f);
}

void ActorModelComponent::render(sf::RenderWindow& window) { window.draw(_sprite); }

void ActorModelComponent::update(double dt)
{
	_sprite.setPosition(_parent->getPosition());
}

//Setting scale factor will set the x and y at which the entity will increase by
void ActorModelComponent::setScaleFactor(float scalefactor)
{
	_sprite.setScale(scalefactor, scalefactor);
}

float ActorModelComponent::getScaleFactor() const
{
	return _sprite.getScale().x;
}