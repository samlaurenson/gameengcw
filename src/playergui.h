#pragma once
#include <SFML/Graphics.hpp>

class PlayerGUI
{
protected:
	PlayerGUI();
	static sf::RectangleShape _hpBar;
	static sf::Text _remainingHP;
	static sf::Font _font;
	static sf::Sprite _atkSpeedBuff;
	static sf::Sprite _atkDamageBuff;
	static bool _atkSpeedBool;
	static bool _atkDamageBool;
	static sf::Text _timeSpent;
public:
	~PlayerGUI() = default;
	static void setHPBar(float health, float hpPool);
	static void initialiseGUI();
	static void showAtkSpeedBuff();
	static void showAtkDamageBuff();

	static void activateAtkSpeedBuff();
	static void activateAtkDamageBuff();

	static void update(const double& dt);
	static void render(sf::RenderWindow& window);
	static void reset();
};