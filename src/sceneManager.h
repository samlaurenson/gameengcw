#include "scene.h"
#include "scores.h"

extern std::shared_ptr<Entity> player;
extern std::vector<std::shared_ptr<Entity>> enemies;
extern std::shared_ptr<Entity> boss;


extern std::shared_ptr<Scene> activeScene;
extern std::shared_ptr<Scene> dungeonScene;
extern std::shared_ptr<Scene> bossScene;
extern std::shared_ptr<Scene> menuScene;
extern std::shared_ptr<Scene> loseScene;
extern std::shared_ptr<Scene> winScene;
extern std::shared_ptr<Scene> leaderboardScene;

extern sf::Clock timer;

class DungeonScene : public Scene {
private:
	sf::Text text;
	sf::Vector2f bossEntrance;

public:
	DungeonScene() = default;
	void update(double dt) override;
	void render(sf::RenderWindow& window) override;
	void load() override;
	void restart() override;
};

class BossScene : public Scene {
private:
	sf::Text text;

public:
	BossScene() = default;
	void update(double dt) override;
	void render(sf::RenderWindow& window) override;
	void load() override;
	void restart() override;
};

class MenuScene : public Scene {
private:	
	sf::Text title;
	sf::Text playText;
	sf::Text leaderboardButton;
public:
	MenuScene() = default;
	void update(double dt) override;
	void render(sf::RenderWindow& window) override;
	void load() override;
};

class LoseScene : public Scene {
private:
	sf::Text loseText;
public:
	LoseScene() = default;
	void update(double dt) override;
	void render(sf::RenderWindow& window) override;
	void load() override;
};

class VictoryScene : public Scene {
private:
	sf::Text winText;
	sf::Text timeTaken;
	static std::string playerInput;
	sf::Text playerText;
	sf::Text submitButton;
public:
	VictoryScene() = default;
	void update(double dt) override;
	void render(sf::RenderWindow & window) override;
	void load() override;
	void restart() override;
	static void appendText(sf::String input);
};

class LeaderboardScene : public Scene {
private:
	static std::string path;
	sf::Text sceneTitle;
	sf::Text leaderboardText;
	sf::Text backButton;
public:
	LeaderboardScene() = default;
	void update(double dt) override;
	void render(sf::RenderWindow & window) override;
	void load() override;
	void restart() override;
	static void setLeaderboardFilePath(std::string path);
	static std::string getLeaderboardFilePath();
};