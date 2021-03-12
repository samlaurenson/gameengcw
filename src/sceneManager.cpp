#include <SFML/Graphics.hpp>
#include <LevelSystem.h>
#include "sceneManager.h"
#include "cmp_shape.h"

std::shared_ptr<Entity> player;

std::shared_ptr<Scene> activeScene;
std::shared_ptr<Scene> dungeonScene;

void Scene::render(sf::RenderWindow& window) { _ents.render(window); }

std::vector<std::shared_ptr<Entity>>& Scene::getEnts() { return _ents.list; }

void Scene::update(double dt) { _ents.update(dt); }

void DungeonScene::load()
{
	auto player = std::make_shared<Entity>();
	auto shape = player->addComponent<ShapeComponent>();
	shape->setShape<sf::CircleShape>(10.f);
	shape->getShape().setPosition(1920 / 2, 1080 / 2);
	shape->getShape().setFillColor(sf::Color::Red);
	shape->getShape().setOrigin(sf::Vector2f(5.f, 5.f));

	_ents.list.push_back(player);

	ls::loadLevelFile("res/dev_level.txt", 32.f);
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