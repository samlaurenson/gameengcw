#include "cmp_actor_model.h"
#include "game.h"

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
