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

	//add component for enemy movement and detection radius for when the enemy will start firing at enemy
	auto model = en->addComponent<ActorModelComponent>();
	model->setModel(sf::IntRect(32, 0, 32, 32));

	_ents.list.push_back(en);
	enemies.push_back(en);

	ls::loadLevelFile("res/dev_level.txt", 32.f);
	restart();
}

void DungeonScene::update(double dt)
{
	Scene::update(dt);
	Bullet::Update(dt);
}

void DungeonScene::render(sf::RenderWindow& window)
{
	ls::Render(window);
	Scene::render(window);
}

//Function to reset player position when dungeon scene is loaded
void DungeonScene::restart()
{
	player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
	player->GetCompatibleComponent<ActorMovementComponent>()[0]->setSpeed(150.f);
	for (int i = 0; i < enemies.size(); i++)
	{
		//Will place enemies as long as the number of enemies is equal to the number of enemy spawns
		enemies[i]->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[i]));
	}
}