#include "playergui.h"
#include "game.h"
#include "sceneManager.h"

sf::RectangleShape PlayerGUI::_hpBar;
sf::Font PlayerGUI::_font;
sf::Text PlayerGUI::_remainingHP;
sf::Text PlayerGUI::_timeSpent;

sf::Sprite PlayerGUI::_atkSpeedBuff;
sf::Sprite PlayerGUI::_atkDamageBuff;

bool PlayerGUI::_atkSpeedBool = false;
bool PlayerGUI::_atkDamageBool = false;

void PlayerGUI::initialiseGUI()
{
	// --- Creating the HP Bar ---
	_hpBar.setSize(sf::Vector2f(400.f, 25.f));
	_hpBar.setFillColor(sf::Color::Red);

	// --- Creating text on HP bar to show exactly how much health is remaining ---
	_font.loadFromFile("res/fonts/Roboto-Medium.ttf");
	_remainingHP.setFont(_font);
	_remainingHP.setFillColor(sf::Color::White);
	_remainingHP.setCharacterSize(24);

	// --- Creating Timer ---
	_timeSpent.setFont(_font);
	_timeSpent.setFillColor(sf::Color::White);
	_timeSpent.setCharacterSize(24);
}

void PlayerGUI::setHPBar(float health, float hpPool)
{
	float healthScale = health / hpPool;
	_hpBar.setScale(healthScale, 1.f);
}

void PlayerGUI::showAtkSpeedBuff()
{
	_atkSpeedBuff.setPosition(Camera.getCenter().x - gameWidth / 2 + 400, Camera.getCenter().y - gameHeight / 2);
	_atkSpeedBuff.setTexture(spritesheet);
	_atkSpeedBuff.setTextureRect(sf::IntRect(32, 0, 32, 32));
}

void PlayerGUI::showAtkDamageBuff()
{
	_atkDamageBuff.setPosition(Camera.getCenter().x - gameWidth / 2 + 440, Camera.getCenter().y - gameHeight / 2);
	_atkDamageBuff.setTexture(spritesheet);
	_atkDamageBuff.setTextureRect(sf::IntRect(0, 0, 32, 32));
}

void PlayerGUI::update(const double& dt)
{
	// --- Setting GUI element positions ---
	_hpBar.setPosition(Camera.getCenter().x - gameWidth/2, Camera.getCenter().y - gameHeight/2);
	_remainingHP.setPosition(Camera.getCenter().x - gameWidth / 2 + 150, Camera.getCenter().y - gameHeight / 2);
	_timeSpent.setPosition(Camera.getCenter().x + gameWidth/2 - 150, Camera.getCenter().y - gameHeight/2);

	_timeSpent.setString(std::to_string(timer.getElapsedTime().asSeconds()));

	// --- Setting health elements (TEXT ON HP BAR AND HP BAR)---
	setHPBar(player->getHealth(), player->getHealthPool());
	
	//if statement to show 0 health if player has negative health, else will show current health
	if (player->getHealth() <= 0)
	{
		_remainingHP.setString("0/" + std::to_string(player->getHealthPool()));
	}
	else {
		_remainingHP.setString(std::to_string(player->getHealth()) + "/" + std::to_string(player->getHealthPool()));
	}

	//If the user has the attack speed buff - then show the attack speed buff in the GUI
	if (_atkSpeedBool)
	{
		showAtkSpeedBuff();
	}
	if (_atkDamageBool)
	{
		showAtkDamageBuff();
	}
}

void PlayerGUI::render(sf::RenderWindow& window)
{
	//draw the hp bar
	window.draw(_hpBar);
	window.draw(_remainingHP);
	window.draw(_atkSpeedBuff);
	window.draw(_atkDamageBuff);
	window.draw(_timeSpent);
}

//using this so that if the player has the attack speed buff and enters another scene - the attack speed buff will not disappear
//look into better way of doing this
void PlayerGUI::activateAtkSpeedBuff()
{
	_atkSpeedBool = true;
}

void PlayerGUI::activateAtkDamageBuff()
{
	_atkDamageBool = true;
}