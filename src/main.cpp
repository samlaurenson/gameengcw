#include <SFML/Graphics.hpp>
#include "sceneManager.h"
#include <LevelSystem.h>

int gameHeight = 1080;
int gameWidth = 1920;

void Load()
{
    //Loading scene assets
    dungeonScene.reset(new DungeonScene());
    dungeonScene->load();

    activeScene = dungeonScene;

    //ls::loadLevelFile("res/dev_level.txt", 64.f);
}

void Update(sf::RenderWindow& window) 
{
    static sf::Clock clock;
    float dt = clock.restart().asSeconds();
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
    //ls::Render(window);
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
