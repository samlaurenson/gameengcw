#include <SFML/Graphics.hpp>
#include "sceneManager.h"
#include <LevelSystem.h>
#include "game.h"
#include "bullet.h"

int gameHeight = 1080;
int gameWidth = 1920;

sf::Texture spritesheet;

sf::View Camera;

void Load()
{
    if (!spritesheet.loadFromFile("res/img/spritesheetEntity2.png"))
    {
        std::cerr << "Failed to load spritesheet!" << std::endl;
    }

    //Loading scene assets
    dungeonScene.reset(new DungeonScene());
    bossScene.reset(new BossScene());
    dungeonScene->load();
    bossScene->load();

    activeScene = dungeonScene;
}

void Update(sf::RenderWindow& window)
{
    static sf::Clock clock;
    float dt = clock.restart().asSeconds();

    activeScene->update(dt);


    //Quick note: May want to have an if statement for the left click to shoot thing for if the active scene is the boss room or dungeon scene if the shoot function will be kept here

    //If statement so that player can only shoot if they are still alive
    if (player->isAlive())
    {
        //Setting player to dead if they have no health
        if (player->getHealth() <= 0)
        {
            player->setAlive(false);
            return;
        }

        static float firetime = 0.0f;
        firetime -= dt;

        //Should probably put this in a better place but will work here for now since a RenderWindow is required
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && firetime <= 0)
        {
            firetime = player->getFirerate();
            sf::Vector2f mousepos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            Bullet::Fire(player->getPosition(), false, mousepos, 25);
        }
    }
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
