#include <SFML/Graphics.hpp>
#include "sceneManager.h"
#include <LevelSystem.h>
#include "game.h"
#include "bullet.h"

int gameHeight = 1080;
int gameWidth = 1920;

sf::Texture spritesheet;
sf::Vector2f mousepos;
sf::View Camera;

void Load()
{
    if (!spritesheet.loadFromFile("res/img/spritesheetEntity2.png"))
    {
        std::cerr << "Failed to load spritesheet!" << std::endl;
    }

    //Loading scene assets
    menuScene.reset(new MenuScene());
    dungeonScene.reset(new DungeonScene());
    bossScene.reset(new BossScene());
    loseScene.reset(new LoseScene());
    menuScene->load();
    dungeonScene->load();
    bossScene->load();
    loseScene->load();

    activeScene = menuScene;
}

void Update(sf::RenderWindow& window)
{
    static sf::Clock clock;
    float dt = clock.restart().asSeconds();

    mousepos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        activeScene = menuScene;

        //Resetting dungeon and boss scenes so all values are the default values and all entities are alive
        dungeonScene->restart();
        bossScene->restart();
    }

    activeScene->update(dt);
}

void Render(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
            return;
        }
    }

    activeScene->render(window);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight), "Trollstigen");
    Load();
    while (window.isOpen())
    {
        window.clear();
        Camera.setSize(gameWidth, gameHeight);
        Camera.setCenter(player->getPosition());
        window.setView(Camera);
        Update(window);
        Render(window);
        window.display();
    }
    return 0;
}
