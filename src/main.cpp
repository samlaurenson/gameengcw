#include <SFML/Graphics.hpp>
#include "sceneManager.h"
#include <LevelSystem.h>
#include "game.h"
#include "bullet.h"

int gameHeight = 1080;
int gameWidth = 1920;

sf::Texture spritesheet;

void Load()
{
    if (!spritesheet.loadFromFile("res/img/spritesheetEntity.png"))
    {
        std::cerr << "Failed to load spritesheet!" << std::endl;
    }

    //Loading scene assets
    dungeonScene.reset(new DungeonScene());
    dungeonScene->load();

    activeScene = dungeonScene;
}

void Update(sf::RenderWindow& window) 
{
    static sf::Clock clock;
    float dt = clock.restart().asSeconds();

    activeScene->update(dt);


    static float firetime = 0.0f;
    firetime -= dt;

    //Should probably put this in a better place but will work here for now since a RenderWindow is required
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && firetime <= 0)
    {
        firetime = player->getFirerate();
        sf::Vector2i mousepos = sf::Mouse::getPosition(window);
        Bullet::Fire(player->getPosition(), false, mousepos, 25);
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
    Bullet::Render(window);
}

int main() 
{
    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight), "Trollstigen");
    Load();
    while (window.isOpen())
    {
        window.clear();
        Update(window);
        Render(window);
        window.display();
    }
    return 0;
}
