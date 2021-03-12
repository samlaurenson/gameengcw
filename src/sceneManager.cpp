#include <SFML/Graphics.hpp>
#include <LevelSystem.h>
#include "sceneManager.h"
#include "cmp_shape.h"
#include "cmp_player_movement.h"

std::shared_ptr<Entity> player;

std::shared_ptr<Scene> activeScene;
std::shared_ptr<Scene> dungeonScene;

void Scene::render(sf::RenderWindow& window) { _ents.render(window); }

std::vector<std::shared_ptr<Entity>>& Scene::getEnts() { return _ents.list; }

void Scene::update(double dt) { _ents.update(dt); }

void DungeonScene::load()
{
	auto pl = std::make_shared<Entity>();

	pl->addComponent<PlayerMovementComponent>();
	auto shape = pl->addComponent<ShapeComponent>();
	shape->setShape<sf::CircleShape>(10.f);
	shape->getShape().setFillColor(sf::Color::Red);
	shape->getShape().setOrigin(sf::Vector2f(5.f, 5.f));

	_ents.list.push_back(pl);
	player = pl;

	ls::loadLevelFile("res/dev_level.txt", 32.f);
	restart();
}

void DungeonScene::update(double dt)
{
	Scene::update(dt);
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
}