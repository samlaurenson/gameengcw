#include "cmp_actor_buff.h"
#include "playergui.h"

ActorBuffComponent::ActorBuffComponent(Entity* p) : Component(p) {}

void ActorBuffComponent::applyBuff(std::shared_ptr<Buff> buff) const
{
	//If enemy has no buff drop
	//NOTE: This may be better done by only having harder enemies access to this function - so make a harder enemy class inherit from enemy which will have the drop buff features instead of regular enemy
	if (buff == nullptr)
	{
		return;
	}

	if (buff->buff == Buff::ATKSPEED)
	{
		_parent->setFirerate(_parent->getFirerate() * buff->modifier);
		PlayerGUI::activateAtkSpeedBuff();
		std::cout << "New firerate: " << _parent->getFirerate() << std::endl;
	}

	if (buff->buff == Buff::ATKDMG)
	{
		_parent->setDamage(_parent->getDamage() * buff->modifier);
		PlayerGUI::activateAtkDamageBuff();
		std::cout << "New Damage: " << _parent->getDamage() << std::endl;
	}
}

void ActorBuffComponent::update(double dt)
{
	
}

void ActorBuffComponent::render(sf::RenderWindow& window)
{

}