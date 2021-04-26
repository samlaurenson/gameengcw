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
#include "playergui.h"
#include "cmp_enemy_movement.h"

#pragma region Entities
std::shared_ptr<Entity> player;
std::vector<std::shared_ptr<Entity>> enemies;
std::shared_ptr<Entity> boss;
#pragma endregion

#pragma region Scenes
std::shared_ptr<Scene> activeScene;
std::shared_ptr<Scene> dungeonScene;
std::shared_ptr<Scene> bossScene;
std::shared_ptr<Scene> menuScene;
std::shared_ptr<Scene> loseScene;
std::shared_ptr<Scene> winScene;
std::shared_ptr<Scene> leaderboardScene;
#pragma endregion

std::string VictoryScene::playerInput;

//Filling array with 10 Scores objects
std::vector<Scores> leaderboard(10, Scores());

std::string LeaderboardScene::path;

sf::Clock timer;
bool gotTime = false; //Variable to flag whether time has been obtained or not
float timeCompleted;

void Scene::render(sf::RenderWindow& window) { _ents.render(window); }

std::vector<std::shared_ptr<Entity>>& Scene::getEnts() { return _ents.list; }

void Scene::update(double dt) { _ents.update(dt); }

void Scene::restart() { gotTime = false; timeCompleted = 0; }

void DungeonScene::load()
{
	//*****Loading in player*****
	auto pl = std::make_shared<Entity>();
	pl->setHealthPool(200);
	pl->setFirerate(0.4f);
	pl->setDamage(25);
	pl->setBulletRange(500.f);

	pl->addComponent<PlayerMovementComponent>();
	auto shape = pl->addComponent<ShapeComponent>();
	shape->setShape<sf::CircleShape>(10.f);
	shape->getShape().setFillColor(sf::Color::Red);
	shape->getShape().setOrigin(sf::Vector2f(5.f, 5.f));
	pl->addComponent<ActorBuffComponent>();
	pl->addComponent<ActorModelComponent>();

	_ents.list.push_back(pl);
	player = pl;

	//*****Loading in regular enemies*****
	//Spawning 25 regular difficulty enemies
	for (int i = 0; i < 25; i++)
	{
		auto en = std::make_shared<Enemy>();
		en->setHealthPool(100);
		en->setBulletRange(700.f);
		en->setDetectionDistance(200.f);
		en->setDamage(25);
		en->setRandomSpawnType(false);
		en->setFirerate(0.6f);
		en->addComponent<EnemyMovementComponent>()->setSpeed(75.f);

		auto model = en->addComponent<ActorModelComponent>();
		model->setModel(sf::IntRect(32, 32, 32, 32));
		model->setScaleFactor(1.f);

		_ents.list.push_back(en);
		enemies.push_back(en);
	}

	//*****Loading hard difficulty enemies*****
	std::vector<std::shared_ptr<Buff>> buffs; //Vector which will store all created buffs

	//Creating and adding buffs to buffs array
	auto b = std::make_shared<Buff>();
	b->buff = Buff::ATKSPEED;
	b->modifier = 0.8f; //0.8f is a 20% increase to player attack speed
	buffs.push_back(b);

	auto b2 = std::make_shared<Buff>();
	b2->buff = Buff::ATKDMG;
	b2->modifier = 2.f; //2.f is double the current player damage - default player damage is 25 so when buff applied will be 50
	buffs.push_back(b2);

	//Looping to create hard difficulty enemies
	for (int i = 0; i < 2; i++)
	{
		auto en = std::make_shared<Enemy>();
		en->setHealthPool(200);
		en->setBulletRange(700.f);
		en->setDetectionDistance(350.f);
		en->setDamage(50);
		en->setRandomSpawnType(true);
		en->setFirerate(0.6f);
		en->addComponent<EnemyMovementComponent>();

		auto model = en->addComponent<ActorModelComponent>();
		model->setModel(sf::IntRect(32, 32, 32, 32));
		model->setScaleFactor(3.f);

		//Assigning a buff that this enemy will drop when they are defeated
		en->addBuffDrop(buffs[i]);

		_ents.list.push_back(en);
		enemies.push_back(en);
	}

	PlayerGUI::initialiseGUI();
}

void DungeonScene::update(double dt)
{
	//If the player walks over the boss room entrance tile then the scene will switch to the boss room
	if (ls::getTileAt(player->getPosition()) == ls::getTileAt(bossEntrance))
	{
		activeScene = bossScene;
		Bullet::clear();
		ls::Unload(); //Unloading any loaded in maps
		bossScene->restart(); //Calling restart on the boss scene to load in maps and set spawn points, boss health, etc
		return;
	}

	if (!player->isAlive())
	{
		activeScene = loseScene;
		ls::Unload();
		return;
	}

	Scene::update(dt);
	Bullet::Update(dt);
	PlayerGUI::update(dt);
}

void DungeonScene::render(sf::RenderWindow& window)
{
	ls::Render(window);
	Bullet::Render(window);
	Scene::render(window);
	PlayerGUI::render(window);
}


//Function to reset player position when dungeon scene is loaded
void DungeonScene::restart()
{
	ls::loadLevelFile("res/lvl_map.txt", 32.f);

	//Clearing scene of bullets and resetting the GUI
	Bullet::clear();
	PlayerGUI::reset();

	//Resetting player values to default and setting spawn point
	player->setHealth(player->getHealthPool());
	player->setAlive(true);
	player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
	player->GetCompatibleComponent<ActorMovementComponent>()[0]->setSpeed(150.f);

	//Setting player damage and fire rate to default in case they picked up any buffs
	player->setFirerate(0.4f);
	player->setDamage(25);


	srand((int)time(0)); //Using time as seed in srand to make randomly generated numbers more random

	//***** SETTING BOSS ROOM ENTRANCE LOCATION *****
#pragma region BossRoomEntranceSpawn

	auto availableBossSpawns = ls::findTiles(ls::RANDOMBOSSSPAWN);
	int rando = rand() % availableBossSpawns.size();
	bossEntrance = ls::getTilePosition(availableBossSpawns[rando]); //Getting the position of the random boss room entrance tile


	//Loop to go over all RANDOMBOSSSPAWN tiles and change tiles to EMPTY and transparent if they are not the boss room entrance tile
	for (int i = 0; i < availableBossSpawns.size(); ++i)
	{
		//If this is not the same tile (checking by same position)
		if (bossEntrance != ls::getTilePosition(availableBossSpawns[i]))
		{
			//Setting all the tiles that are not boss room entrance tiles to empty tiles and making them transparent
			ls::setTileType(availableBossSpawns[i], ls::EMPTY);
			ls::setColor(ls::getTileAt(ls::getTilePosition(availableBossSpawns[i])), sf::Color::Transparent);
		}
	}
#pragma endregion

	//***** SETTING ENEMY SPAWN LOCATIONS *****
#pragma region EnemySpawns
	auto randEnemyTiles = ls::findTiles(ls::RANDOMENEMY); //Getting an array of all RANDOMENEMY tiles

	//could add a type to enemy entity to check whether they are a regular or hard enemy type so if enemy is regular spawn on enemy tile, if enemy is hard spawn on randomenemy tile
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->setHealth(enemies[i]->getHealthPool());
		enemies[i]->setAlive(true);
		
		//Placeholder for spawning enemies - if the enemy is a random spawn type, it will randomly select a ENEMYRANDOM tile and will spawn the enemy there and remove the tile from the vector
		//to prevent other enemies from spawning in the same space
		//If not random spawn type, then enemies will spawn on all set tiles
		if (enemies[i]->getRandomSpawnType())
		{
			int random = rand() % randEnemyTiles.size();
			enemies[i]->setPosition(ls::getTilePosition(randEnemyTiles[random]));
			randEnemyTiles.erase(randEnemyTiles.begin() + random);
		}
		else {
			//Will place enemies as long as there are enough spawn points for enemies
			enemies[i]->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[i]));
		}
	}
#pragma endregion

	Scene::restart();
}

void BossScene::load()
{
	_ents.list.push_back(player);

	//may be cool to add actor buff component to boss where they have a higher fire rate when they reach 50% health
	//so in update function - when boss health is less than 50% of its original health - apply attack speed buff
	auto b = std::make_shared<Enemy>();
	b->setHealthPool(20);
	b->setBulletRange(5000.f);
	b->setDetectionDistance(1000);
	b->setFirerate(0.2f);
	b->setDamage(75);
	auto model = b->addComponent<ActorModelComponent>();
	model->setModel(sf::IntRect(32, 32, 32, 32));
	model->setScaleFactor(10.f);

	_ents.list.push_back(b);
	boss = b;

	PlayerGUI::initialiseGUI();
}


void BossScene::update(double dt)
{
	if (!boss->isAlive() && !gotTime)
	{
		gotTime = true;
		timeCompleted = timer.getElapsedTime().asSeconds();
		std::cout << "Completed dungeon in: " << timeCompleted << std::endl;

		activeScene = winScene;  //After boss has been defeated - player has won
		ls::Unload();
		return;
	}

	if (!player->isAlive())
	{
		activeScene = loseScene;
		ls::Unload();
		return;
	}

	Bullet::Update(dt);
	Scene::update(dt);
	PlayerGUI::update(dt);
}

void BossScene::render(sf::RenderWindow& window)
{
	ls::Render(window);
	Bullet::Render(window);
	Scene::render(window);
	PlayerGUI::render(window);
}

void BossScene::restart()
{
	ls::loadLevelFile("res/dev_level2.txt", 32.f); //Loading in the level

	//Setting player and boss spawn locations
	boss->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[0]));
	player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));

	//Resetting boss health and alive status
	boss->setHealth(boss->getHealthPool());
	boss->setAlive(true);
}

void MenuScene::load()
{
	//Set title
	font.loadFromFile("res/fonts/Roboto-Medium.ttf");
	title.setFont(font);
	title.setFillColor(sf::Color::White);
	title.setCharacterSize(36);
	title.setPosition(gameWidth / 3, gameHeight / 3);
	title.setString("Trollstigen");

	//Set play button
	playText.setFont(font);
	playText.setFillColor(sf::Color::White);
	playText.setCharacterSize(28);
	playText.setOrigin(playText.getGlobalBounds().left + round(playText.getGlobalBounds().width / 2), playText.getGlobalBounds().top + round(playText.getGlobalBounds().height / 2));
	playText.setPosition(gameWidth / 10, gameHeight / 2);
	playText.setString("Play Game");

	//Set leaderboard button
	leaderboardButton.setFont(font);
	leaderboardButton.setFillColor(sf::Color::White);
	leaderboardButton.setCharacterSize(28);
	leaderboardButton.setPosition(gameWidth / 10, gameHeight / 1.5);
	leaderboardButton.setString("Leaderboards");
}

void MenuScene::update(double dt)
{
	//Mouse events for play "button" - Hover over button and clicking on button
	if (mousepos.x > playText.getPosition().x && mousepos.x < playText.getPosition().x + 200 && mousepos.y > playText.getPosition().y && mousepos.y < playText.getPosition().y + 50)
	{
		playText.setFillColor(sf::Color::Yellow);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			dungeonScene->restart(); //restarting dungeonscene to load in map and then switch to dungeon scene
			activeScene = dungeonScene;
			timer.restart();
		}
	}
	else {
		playText.setFillColor(sf::Color::White);
	}

	if (mousepos.x > leaderboardButton.getPosition().x && mousepos.x < leaderboardButton.getPosition().x + 200 && mousepos.y > leaderboardButton.getPosition().y && mousepos.y < leaderboardButton.getPosition().y + 50)
	{
		leaderboardButton.setFillColor(sf::Color::Yellow);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			leaderboardScene->restart();
			activeScene = leaderboardScene;
		}
	}
	else {
		leaderboardButton.setFillColor(sf::Color::White);
	}

	Scene::update(dt);
}

void MenuScene::render(sf::RenderWindow& window)
{
	window.draw(title);
	window.draw(playText);
	window.draw(leaderboardButton);
	Scene::render(window);
}

void LoseScene::load()
{
	//Set lose text
	font.loadFromFile("res/fonts/Roboto-Medium.ttf");
	loseText.setFont(font);
	loseText.setFillColor(sf::Color::White);
	loseText.setCharacterSize(36);
	loseText.setPosition(gameWidth / 2 - 75, gameHeight / 2);
	loseText.setString("Oh dear, you died.");
}

void LoseScene::update(double dt)
{
	Scene::update(dt);
}

void LoseScene::render(sf::RenderWindow& window)
{
	window.draw(loseText);
	Scene::render(window);
}

void VictoryScene::load()
{
	//Text to say that player has won
	font.loadFromFile("res/fonts/Roboto-Medium.ttf");
	winText.setFont(font);
	winText.setFillColor(sf::Color::White);
	winText.setCharacterSize(36);
	winText.setPosition(gameWidth / 2 - 75, gameHeight / 3);
	winText.setString("Well Done!\n You Defeated the Boss!");

	//Text to show time user spent playing
	timeTaken.setFont(font);
	timeTaken.setFillColor(sf::Color::White);
	timeTaken.setCharacterSize(36);
	timeTaken.setPosition(gameWidth / 2 - 75, gameHeight / 2);

	//Setting up text to show player input
	playerText.setFont(font);
	playerText.setFillColor(sf::Color::White);
	playerText.setCharacterSize(36);
	playerText.setPosition(gameWidth / 2 - 75, gameHeight / 1.25);
	playerText.setString("Enter Name: ");

	//Setting up submit button
	submitButton.setFont(font);
	submitButton.setFillColor(sf::Color::White);
	submitButton.setCharacterSize(36);
	submitButton.setPosition(gameWidth / 2 - 75, gameHeight);
	submitButton.setString("Submit");
}

void VictoryScene::update(double dt)
{
	timeTaken.setString("Time Taken: " + std::to_string(timeCompleted));

	playerText.setString("Enter Name: " + playerInput);

	//Need if for when user presess "submit" button to save the scores and then go back to menu page
	if (mousepos.x > submitButton.getPosition().x && mousepos.x < submitButton.getPosition().x + 200 && mousepos.y > submitButton.getPosition().y && mousepos.y < submitButton.getPosition().y + 50)
	{
		submitButton.setFillColor(sf::Color::Yellow);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			Scores newScore;
			newScore.setTime(timeCompleted);
			newScore.setPlayerName(playerInput);

			//Insert into vector where time is greater than another time
			for (int i = 0; i < leaderboard.size(); i++)
			{
				if (newScore.getTime() < leaderboard[i].getTime() || leaderboard[i].getTime() == 0.f)
				{
					//Should insert the score in correct place depending on if time is faster
					leaderboard.insert(leaderboard.begin() + i, newScore);

					//If leaderboard already has 10 entries, then remove the last one
					if (leaderboard.size() > 10)
					{
						leaderboard.erase(leaderboard.begin() + leaderboard.size() - 1);
					}
					break;
				}
			}
			//leaderboard.push_back(newScore);

			Scores::saveScoresToFile(leaderboard, LeaderboardScene::getLeaderboardFilePath());
			leaderboardScene->restart();

			//LeaderboardScene::buildLeaderboard(leaderboard);

			activeScene = menuScene;

			//Resetting dungeon and boss scenes so all values are the default values and all entities are alive
			dungeonScene->restart();
			bossScene->restart();

			//resetting victory screen
			winScene->restart();
		}
	}
	else {
		submitButton.setFillColor(sf::Color::White);
	}

	Scene::update(dt);
}

void VictoryScene::render(sf::RenderWindow& window)
{
	window.draw(winText);
	window.draw(timeTaken);
	window.draw(playerText);
	window.draw(submitButton);
	Scene::render(window);
}

//Function that will add characters to a string when user provides keyboard input on victory screen
void VictoryScene::appendText(sf::String inputText)
{
	playerInput += inputText;
}

void VictoryScene::restart()
{
	playerInput = "";
	playerText.setString("Enter Name: ");
}

void LeaderboardScene::load()
{
	//Setting path to leaderboard
	LeaderboardScene::setLeaderboardFilePath("res/leaderboard.scores");

	font.loadFromFile("res/fonts/Roboto-Medium.ttf");

	//Setting leaderboard title
	sceneTitle.setFont(font);
	sceneTitle.setFillColor(sf::Color::White);
	sceneTitle.setCharacterSize(36);
	sceneTitle.setPosition(gameWidth / 3, gameHeight / 3);
	sceneTitle.setString("Top 10 Leaderboard");

	//Setting leaderboard font
	leaderboardText.setFont(font);
	leaderboardText.setFillColor(sf::Color::White);
	leaderboardText.setCharacterSize(36);
	leaderboardText.setPosition(gameWidth / 3, gameHeight / 2);

	//Setting back button that will take user back to main menu
	backButton.setFont(font);
	backButton.setFillColor(sf::Color::White);
	backButton.setCharacterSize(28);
	backButton.setPosition(gameWidth / 15, gameHeight / 0.9);
	backButton.setString("< Back");

	//Reading in scores from file and adding them to the leaderboard vector
	leaderboard = Scores::loadScoresFromFile(getLeaderboardFilePath());
}

void LeaderboardScene::update(double dt)
{
	//Creating event that will highlight button when mouse is hovering over it
	if (mousepos.x > backButton.getPosition().x && mousepos.x < backButton.getPosition().x + 200 && mousepos.y > backButton.getPosition().y && mousepos.y < backButton.getPosition().y + 50)
	{
		backButton.setFillColor(sf::Color::Yellow);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			activeScene = menuScene;
		}
	}
	else {
		backButton.setFillColor(sf::Color::White);
	}

	Scene::update(dt);
}

void LeaderboardScene::render(sf::RenderWindow& window)
{
	window.draw(sceneTitle);
	window.draw(leaderboardText);
	window.draw(backButton);
	Scene::render(window);
}

void LeaderboardScene::setLeaderboardFilePath(std::string _path)
{
	path = _path;
}

std::string LeaderboardScene::getLeaderboardFilePath()
{
	return path;
}

void LeaderboardScene::restart()
{
	std::string resultText = "";

	for (int i = 0; i < leaderboard.size(); i++)
	{
		if (leaderboard[i].getTime() != 0.f)
		{
			resultText.append(std::to_string(i + 1) + ".\t" + leaderboard[i].getPlayerName() + "\t" + std::to_string(leaderboard[i].getTime()) + "\n");
		}
		else {
			resultText.append(std::to_string(i + 1) + ".\t" + "...." + "\t" + "...." + "\n");
		}
	}

	leaderboardText.setString(resultText);
}