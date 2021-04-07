#include <SFML/Graphics.hpp>
#include <LevelSystem.h>
#include "sceneManager.h"
#include "cmp_shape.h"
#include "cmp_player_movement.h"
#include "game.h"
#include "bullet.h"
#include "cmp_actor_model.h"
#include "enemy.h"
#include "cmp_actor_buff.h"
#include "playergui.h"
#include "cmp_enemy_movement.h"

std::shared_ptr<Entity> player;
std::vector<std::shared_ptr<Entity>> enemies;
std::shared_ptr<Entity> boss;

std::shared_ptr<Scene> activeScene;
std::shared_ptr<Scene> dungeonScene;
std::shared_ptr<Scene> bossScene;
std::shared_ptr<Scene> menuScene;
std::shared_ptr<Scene> loseScene;
std::shared_ptr<Scene> winScene;

sf::Clock timer;

void Scene::render(sf::RenderWindow& window) { _ents.render(window); }

std::vector<std::shared_ptr<Entity>>& Scene::getEnts() { return _ents.list; }

void Scene::update(double dt) { _ents.update(dt); }

void Scene::restart() { }

void DungeonScene::load()
{
	timer.restart();

	//Loading in player
	auto pl = std::make_shared<Entity>();
	pl->setHealthPool(200);
	pl->setFirerate(0.4f);
	pl->setDamage(25);
	pl->setBulletRange(500.f);

	pl->addComponent<PlayerMovementComponent>();
	auto shape = pl->addComponent<ShapeComponent>();
	shape->setShape<sf::CircleShape>(10.f);
	shape->getShape().setFillColor(sf::Color::Red);
	shape->getShape().setOrigin(sf::Vector2f(5.f, 5.f));
	pl->addComponent<ActorBuffComponent>();
	pl->addComponent<ActorModelComponent>();

	_ents.list.push_back(pl);
	player = pl;

	//Loading enemies
	auto en = std::make_shared<Enemy>();
	en->setHealthPool(100);
	en->setBulletRange(700.f);
	en->setDetectionDistance(350.f);
	en->setDamage(25);
	en->setRandomSpawnType(true);

	auto b = std::make_shared<Buff>();
	b->buff = Buff::ATKSPEED;
	b->modifier = 0.8f; //0.8f is a 20% increase to player attack speed
	en->addBuffDrop(b);
	en->setFirerate(0.6f);
	en->addComponent<EnemyMovementComponent>();

	//add component for enemy movement and detection radius for when the enemy will start firing at enemy
	auto model = en->addComponent<ActorModelComponent>();
	model->setModel(sf::IntRect(32, 32, 32, 32));
	model->setScaleFactor(3.f);

	_ents.list.push_back(en);
	enemies.push_back(en);

	auto en2 = std::make_shared<Enemy>();
	en2->setHealthPool(100);
	en2->setBulletRange(700.f);
	en2->setDetectionDistance(350.f);
	en2->setDamage(25);
	en2->setRandomSpawnType(true);
	en2->addComponent<EnemyMovementComponent>();
	auto b2 = std::make_shared<Buff>();
	b2->buff = Buff::ATKDMG;
	b2->modifier = 2.f; //2.f is double the current player damage - default player damage is 25 so when buff applied will be 50
	en2->addBuffDrop(b2);
	en2->setFirerate(0.6f);
	auto model2 = en2->addComponent<ActorModelComponent>();
	model2->setModel(sf::IntRect(32, 32, 32, 32));
	model2->setScaleFactor(3.f);
	_ents.list.push_back(en2);
	enemies.push_back(en2);



	ls::loadLevelFile("res/dev_level.txt", 32.f);
	PlayerGUI::initialiseGUI();
	restart();
}

void DungeonScene::update(double dt)
{
	//Using this to check transition between dungeon scene and boss scene (i.e. all bullets clear off screen and player and boss load)
	//Replace this with if the user runs over the boss room entrance
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		activeScene = bossScene;
		Bullet::clear();
		//restart();
	}

	if (!player->isAlive())
	{
		activeScene = loseScene;
	}

	Scene::update(dt);
	Bullet::Update(dt);
	PlayerGUI::update(dt);
}

void DungeonScene::render(sf::RenderWindow& window)
{
	ls::Render(window);
	Bullet::Render(window);
	Scene::render(window);
	PlayerGUI::render(window);
}

//Function to reset player position when dungeon scene is loaded
void DungeonScene::restart()
{
	srand((int)time(0)); //Using time as seed in srand to make randomly generated numbers more random

	//Clearing scene of bullets and resetting the GUI
	Bullet::clear();
	PlayerGUI::reset();

	//Also reset player and enemy health here for when scenes are restarted
	player->setHealth(player->getHealthPool());
	player->setAlive(true);
	//player->setFirerate(0.4f);
	player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
	player->GetCompatibleComponent<ActorMovementComponent>()[0]->setSpeed(150.f);

	//Setting player damage and fire rate to default in case they picked up any buffs
	player->setFirerate(0.4f);
	player->setDamage(25);

	auto enemyTiles = ls::findTiles(ls::ENEMY); //create new tile type for ENEMYRANDOM

	//could add a type to enemy entity to check whether they are a regular or hard enemy type so if enemy is regular spawn on enemy tile, if enemy is hard spawn on randomenemy tile
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->setHealth(enemies[i]->getHealthPool());
		enemies[i]->setAlive(true);
		
		//Placeholder for spawning enemies - if the enemy is a random spawn type, it will randomly select a ENEMYRANDOM tile and will spawn the enemy there and remove the tile from the vector
		//to prevent other enemies from spawning in the same space
		//If not random spawn type, then enemies will spawn on all set tiles
		if (enemies[i]->getRandomSpawnType())
		{
			int random = rand() % enemyTiles.size();
			enemies[i]->setPosition(ls::getTilePosition(enemyTiles[random]));
			enemyTiles.erase(enemyTiles.begin() + random);
		}
		else {
			//Will place enemies as long as there are enough spawn points for enemies
			enemies[i]->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[i]));
		}
	}
}

void BossScene::load()
{
	_ents.list.push_back(player);

	//may be cool to add actor buff component to boss where they have a higher fire rate when they reach 50% health
	//so in update function - when boss health is less than 50% of its original health - apply attack speed buff
	auto b = std::make_shared<Enemy>();
	b->setHealth(2000);
	b->setBulletRange(5000.f);
	b->setDetectionDistance(1000);
	b->setFirerate(0.2f);
	b->setDamage(75);
	auto model = b->addComponent<ActorModelComponent>();
	model->setModel(sf::IntRect(32, 32, 32, 32));
	model->setScaleFactor(10.f);

	_ents.list.push_back(b);
	boss = b;

	PlayerGUI::initialiseGUI();
	ls::loadLevelFile("res/dev_level.txt", 32.f);
	boss->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[0]));
}

bool gotTime = false; //Variable to flag whether time has been obtained or not

void BossScene::update(double dt)
{
	//Using this to test that dungeon scene will reset correctly when opened
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		activeScene = dungeonScene;
		Bullet::clear();
	}

	float timeCompleted;
	if (!boss->isAlive() && !gotTime)
	{
		gotTime = true;
		timeCompleted = timer.getElapsedTime().asSeconds();
		std::cout << "Completed dungeon in: " << timeCompleted << std::endl;
	}

	if (!player->isAlive())
	{
		activeScene = loseScene;
	}

	Bullet::Update(dt);
	Scene::update(dt);
	PlayerGUI::update(dt);
}

void BossScene::render(sf::RenderWindow& window)
{
	ls::Render(window);
	Bullet::Render(window);
	Scene::render(window);
	PlayerGUI::render(window);
}

void BossScene::restart()
{
	boss->setHealth(boss->getHealthPool());
	boss->setAlive(true);
}

void MenuScene::load()
{
	//Set title
	font.loadFromFile("res/fonts/Roboto-Medium.ttf");
	title.setFont(font);
	title.setFillColor(sf::Color::White);
	title.setCharacterSize(36);
	title.setPosition(gameWidth / 3, gameHeight / 3);
	title.setString("Trollstigen");

	//Set play button
	playText.setFont(font);
	playText.setFillColor(sf::Color::White);
	playText.setCharacterSize(28);
	playText.setPosition(gameWidth / 10, gameHeight / 2);
	playText.setString("Play Game");
}

void MenuScene::update(double dt)
{
	//Mouse events for play "button" - Hover over button and clicking on button
	if (mousepos.x > playText.getPosition().x && mousepos.x < playText.getPosition().x + 200 && mousepos.y > playText.getPosition().y && mousepos.y < playText.getPosition().y + 50)
	{
		playText.setFillColor(sf::Color::Yellow);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			activeScene = dungeonScene;
		}
	}
	else {
		playText.setFillColor(sf::Color::White);
	}

	Scene::update(dt);
}

void MenuScene::render(sf::RenderWindow& window)
{
	window.draw(title);
	window.draw(playText);
	Scene::render(window);
}

void LoseScene::load()
{
	//Set lose text
	font.loadFromFile("res/fonts/Roboto-Medium.ttf");
	loseText.setFont(font);
	loseText.setFillColor(sf::Color::White);
	loseText.setCharacterSize(36);
	loseText.setPosition(gameWidth / 2 - 75, gameHeight / 2);
	loseText.setString("Oh dear, you died.");
}

void LoseScene::update(double dt)
{
	Scene::update(dt);
}

void LoseScene::render(sf::RenderWindow& window)
{
	window.draw(loseText);
	Scene::render(window);
}