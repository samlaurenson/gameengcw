#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "sceneManager.h"
#include <LevelSystem.h>
#include "game.h"

int gameHeight = 1080;
int gameWidth = 1920;

sf::Font font;
sf::Sound playershootaudio;
sf::Sound bossroaraudio;
sf::SoundBuffer playshootbuffer;
sf::SoundBuffer bossroarbuffer;
sf::Texture spritesheet;
sf::Vector2f mousepos;
sf::View Camera;


void Load()
{
    // If statements to check audio files and sprite sheet load correcty.
    if (!bossroarbuffer.loadFromFile("res/Audio/mixkit-giant-monster-roar-1972.wav"))
    {
        std::cerr << "Failed to load boss audio!" << std::endl;
    }
    
    if (!playshootbuffer.loadFromFile("res/Audio/270343__littlerobotsoundfactory__shoot-01.wav"))
    {
        std::cerr << "Failed to load player audio!" << std::endl;
    }
    if (!spritesheet.loadFromFile("res/img/spritesheetEntity2.png"))
    {
        std::cerr << "Failed to load spritesheet!" << std::endl;
    }

    font.loadFromFile("res/fonts/Roboto-Medium.ttf");

    bossroaraudio.setBuffer(bossroarbuffer);
    playershootaudio.setBuffer(playshootbuffer);

    

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

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && activeScene != menuScene)
    {
        activeScene = menuScene;
        ls::Unload(); //Unloading any maps that are currently loaded in upon returning to main menu

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
        //If statement to check whether the scene is a menu scene or not - if it's not a menu scene then the camera will follow the player
        //If it is a menu scene then the camera will be set to a specific location
        if (activeScene == menuScene || activeScene == leaderboardScene || activeScene == winScene || activeScene == loseScene)
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
