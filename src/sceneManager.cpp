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

std::shared_ptr<Entity> player;
std::vector<std::shared_ptr<Entity>> enemies;

std::shared_ptr<Scene> activeScene;
std::shared_ptr<Scene> dungeonScene;
std::shared_ptr<Scene> bossScene;

void Scene::render(sf::RenderWindow& window) { _ents.render(window); }

std::vector<std::shared_ptr<Entity>>& Scene::getEnts() { return _ents.list; }

void Scene::update(double dt) { _ents.update(dt); }

void DungeonScene::load()
{
	//Loading in player
	auto pl = std::make_shared<Entity>();
	pl->setHealth(200);
	pl->setFirerate(0.4f);

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
	en->setHealth(100);
	en->setDetectionDistance(350.f);

	auto b = std::make_shared<Buff>();
	b->buff = Buff::ATKSPEED;
	b->modifier = 0.8f; //0.8f is a 20% increase to player attack speed
	en->addBuffDrop(b);
	en->setFirerate(0.6f);

	//add component for enemy movement and detection radius for when the enemy will start firing at enemy
	auto model = en->addComponent<ActorModelComponent>();
	model->setModel(sf::IntRect(32, 0, 32, 32));
	model->setScaleFactor(3.f);

	_ents.list.push_back(en);
	enemies.push_back(en);

	ls::loadLevelFile("res/dev_level.txt", 32.f);
	restart();
}

void DungeonScene::update(double dt)
{
	//Using this to check transition between dungeon scene and boss scene (i.e. all bullets clear off screen and player and boss load)
	//Replace this with if the user runs over the boss room entrance
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		activeScene = bossScene;
		Bullet::clear();
	}
	Scene::update(dt);
	Bullet::Update(dt);
}

void DungeonScene::render(sf::RenderWindow& window)
{
	ls::Render(window);
	Bullet::Render(window);
	Scene::render(window);
}

//Function to reset player position when dungeon scene is loaded
void DungeonScene::restart()
{
	//Also reset player and enemy health here for when scenes are restarted
	player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
	player->GetCompatibleComponent<ActorMovementComponent>()[0]->setSpeed(150.f);

	//could add a type to enemy entity to check whether they are a regular or hard enemy type so if enemy is regular spawn on enemy tile, if enemy is hard spawn on randomenemy tile
	for (int i = 0; i < enemies.size(); i++)
	{
		//Will place enemies as long as the number of enemies is equal to the number of enemy spawns
		enemies[i]->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[i]));
	}
}

void BossScene::load()
{
	_ents.list.push_back(player);

	//may be cool to add actor buff component to boss where they have a higher fire rate when they reach 50% health
	//so in update function - when boss health is less than 50% of its original health - apply attack speed buff
	auto boss = std::make_shared<Enemy>();
	boss->setHealth(1000);
	boss->setDetectionDistance(1000);
	boss->setFirerate(0.2f);
	auto model = boss->addComponent<ActorModelComponent>();
	model->setModel(sf::IntRect(32, 0, 32, 32));
	model->setScaleFactor(10.f);

	_ents.list.push_back(boss);

	ls::loadLevelFile("res/dev_level.txt", 32.f);
	boss->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[0]));
}

void BossScene::update(double dt)
{
	//Using this to test that dungeon scene will reset correctly when opened
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		activeScene = dungeonScene;
		Bullet::clear();
	}
	Bullet::Update(dt);
	Scene::update(dt);
}

void BossScene::render(sf::RenderWindow& window)
{
	ls::Render(window);
	Bullet::Render(window);
	Scene::render(window);
}