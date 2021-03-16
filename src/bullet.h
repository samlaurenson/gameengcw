#pragma once
#include <../lib_ecm/ecm.h>

class Bullet : public sf::Sprite {
public:
	static void Update(const float& dt);
	static void Render(sf::RenderWindow& window);
	static void Fire(const sf::Vector2f& pos, const bool mode, sf::Vector2i direction);
	~Bullet() = default;
protected:
	Bullet(); //Never called by our code
	static unsigned char bulletPointer;
	static Bullet bullets[256];
	void _Update(const float& dt); //called by the static Update()
	bool _mode; //false -> player bullet, true -> enemy bullet
	bool _fired;
	sf::Vector2i direction;
};