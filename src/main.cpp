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
    winScene.reset(new VictoryScene());
    leaderboardScene.reset(new LeaderboardScene());
    menuScene->load();
    dungeonScene->load();
    bossScene->load();
    loseScene->load();
    winScene->load();
    leaderboardScene->load();


    //Setting path for leaderboard
    //LeaderboardScene::setLeaderboardFilePath("res/leaderboard.scores");

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

        //resetting victory screen
        winScene->restart();
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

        //Would be useful to add backspace in here
        //Used to get the player input when player is on the victory screen
        if (event.type == sf::Event::TextEntered && activeScene == winScene)
        {
            //nameInput += event.text.unicode;
            VictoryScene::appendText(event.text.unicode);
        }
    }

    activeScene->render(window);
}

int main()
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    //sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight), "Trollstigen");
    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, desktop.bitsPerPixel), "Trollstigen");
    Load();
    while (window.isOpen())
    {
        window.clear();
        Camera.setSize(gameWidth, gameHeight);
        if (activeScene == menuScene || activeScene == leaderboardScene)
        {
            Camera.setCenter(gameWidth / 2, gameHeight * 0.65);
        }
        else {
            Camera.setCenter(player->getPosition());
        }
        window.setView(Camera);
        Update(window);
        Render(window);
        window.display();
    }
    return 0;
}
